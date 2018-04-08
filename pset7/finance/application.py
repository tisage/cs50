from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    # get all symbol and shares sets from database
    portfolios = db.execute("SELECT shares, symbol FROM portfolio WHERE id=:id", id=session["user_id"])
    
    # add initial investment money option
    tempInvestment = db.execute("SELECT initialInvestment FROM users WHERE id=:id", id=session["user_id"])
    initialInvestment = float(tempInvestment[0]["initialInvestment"])
    # temporary variable
    totalCash = 0
    
    # summarize and update each symbol prices and its total
    for currentStock in portfolios:
        symbol=currentStock["symbol"]
        shares=currentStock["shares"]
        stock=lookup(symbol)
        total=stock["price"] * shares
        totalCash += total
        # update symbol and total portfolio database
        db.execute("UPDATE portfolio SET price=:price, total=:total WHERE id=:id AND symbol=:symbol", \
                        price=usd(stock["price"]), total=usd(total), id=session["user_id"], symbol=symbol)
    
    # Attention on the indentation here, the following should be the update in database, not adding up total money in loop()
    # update cash in portfolio
    updatedCash = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
    marketCap = totalCash
    # update total cash
    totalCash += updatedCash[0]["cash"]
        
    # Display on index page
    updatedPortfolio=db.execute("SELECT * from portfolio WHERE id=:id", id=session["user_id"])
    # Rate Calculation
    capitalGain = totalCash - initialInvestment
    roi = round(capitalGain/initialInvestment * 100, 2)
    return render_template("index.html", stocks=updatedPortfolio, initialInvestment=usd(initialInvestment), marketCap=usd(marketCap), cash=usd(updatedCash[0]["cash"]), total=usd(totalCash), capitalGain=usd(capitalGain), roi=roi)
        
    #return apology("TODO")

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method== "GET":
        return render_template("buy.html")
    else:
        # check symbol input
        stock = lookup(request.form.get("symbol"))
        if not stock:
            return apology("Invalid Symbol")
        
        # get shares and check input
        try:
            shares = int(request.form.get("shares"))
            if shares < 0:
                return apology("Invalid Share Number")
        except:
            return apology("Share Number Should be Positive Integer")
            
        # select user's current cash, named money
        money = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
        
        # check enough cash
        if not money or float(money[0]["cash"]) < stock["price"] * shares:
            return apology("Sorry, not enough cash")
            
        # afordable, then update a new database, database named histories will be used in index page
        db.execute("INSERT INTO histories (id, symbol, shares, price) VALUES(:id, :symbol, :shares, :price)", \
                       id=session["user_id"], symbol=stock["symbol"], shares=shares, price=usd(stock["price"]))
        
        # update cash database
        db.execute("UPDATE users SET cash = cash - :purchase WHERE id=:id", id=session["user_id"], \
                        purchase = stock["price"]* shares)
                        
        # Check shares of this specific symbol
        # Select first
        userShares = db.execute("SELECT shares FROM portfolio WHERE id=:id AND symbol=:symbol", \
                        id=session["user_id"], symbol=stock["symbol"])
        
        # Check user database whether we have this symbol before, if not, create one
        if not userShares:
            db.execute("INSERT INTO portfolio (id, symbol, name, shares, price, total) \
                        VALUES(:id, :symbol, :name, :shares, :price, :total)", id=session["user_id"], symbol=stock["symbol"], \
                        name=stock["name"], shares=shares, price=usd(stock["price"]), total=usd(stock["price"]*shares))
        else:
            # just increase the number of shares
            sharesTotal=userShares[0]["shares"]+shares
            db.execute("UPDATE portfolio SET shares=:shares WHERE id=:id AND symbol=:symbol", shares=sharesTotal, id=session["user_id"],symbol=stock["symbol"])
    
        return redirect(url_for("index"))
    # return apology("TODO")
    
@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    
    # Just a SELECT * query statement from histories database for current user
    histories = db.execute("SELECT * FROM histories WHERE id=:id", id=session["user_id"])
    
    return render_template("history.html", histories = histories)
    #return apology("TODO")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("Invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    if request.method == "POST":
        stock=lookup(request.form.get("symbol"))
        
        if not stock:
            return apology("Invalid Symbol")
        return render_template("quoted.html", stock=stock)
        
    else:
        return render_template("quote.html")
    #return apology("TODO")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    '''
        Require that a user input a username. 
        Render an apology if the user’s input is blank or the username already exists.
    
        Require that a user input a password and then that same password again. 
        Render an apology if either input is blank or the passwords do not match.
    
        INSERT the new user into users, 
        storing a hash of the user’s password, not the password itself. 
        Odds are you’ll find pwd_context.hash of interest.
    '''   
    # if user reached route via POST (as by submitting a form via POST)
    
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")
        
        # ensure two passwords do match
        elif request.form.get("password") != request.form.get("password_again"):
            return apology("password not match")
        
        initialInvestment = float(request.form.get("initialInvestment"))
        # ensure investment money
        if not initialInvestment:
            return apology("must provide Initial Money")
        elif initialInvestment <=0:
            return apology("Initial Money must provide be positve number")
            
        # create username in database
        result = db.execute("INSERT INTO users (username, hash, cash, initialInvestment) VALUES(:username, :hash, :cash, :initialInvestment)", \
            username = request.form.get("username"), hash = pwd_context.hash(request.form.get("password")), cash=initialInvestment, initialInvestment=initialInvestment)
        
        if not result:
            return apology("username already exist")
            
        # remember which user has logged in
        session["user_id"] = result

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")
    #return apology("TODO")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method== "GET":
        return render_template("sell.html")
    else:
        # check symbol input
        stock = lookup(request.form.get("symbol"))
        if not stock:
            return apology("Invalid Symbol")
        
        # get shares and check input
        try:
            shares = int(request.form.get("shares"))
            if shares < 0:
                return apology("Invalid Share Number")
        except:
            return apology("Share Number Should be Positive Integer")
            

        # Check shares of this specific symbol
        # Select first
        userShares = db.execute("SELECT shares FROM portfolio WHERE id=:id AND symbol=:symbol", \
                        id=session["user_id"], symbol=stock["symbol"])
        
        # Check user database whether we have enough shares to sell
        if not userShares or userShares[0]["shares"] < shares:
            return apology("Sorry, not enough shares to sell")
        else:    
            # Update the records in histories database
            db.execute("INSERT INTO histories (id, symbol, shares, price) VALUES (:id, :symbol, :shares, :price)", \
            id=session["user_id"], symbol=stock["symbol"], shares=-shares, price=usd(stock["price"]))
            
            # Update portfolio database
            # just decrease the number of shares
            sharesTotal=userShares[0]["shares"]-shares
            
            # delete records from portfolio
            if sharesTotal == 0:
                db.execute("DELETE FROM portfolio WHERE id=:id AND symbol=:symbol", \
                            id=session["user_id"], symbol=stock["symbol"])
            
            # otherwise, modify shares number
            else:    
                db.execute("UPDATE portfolio SET shares=:shares WHERE id=:id AND symbol=:symbol", shares=sharesTotal, id=session["user_id"],symbol=stock["symbol"])
            
            # Update cash database
            db.execute("UPDATE users SET cash = cash + :purchase WHERE id=:id", id=session["user_id"], \
                        purchase = stock["price"]* shares)
                        
            return redirect(url_for("index"))
    #return apology("TODO")

# Customized Funcitons To-Do
# @app.route("/loan", method["GET, POST"])
# @login_required

# @app.route("/Chart", method["GET, POST"])
# @login_required

# @app.route("/Bechmark", method["GET, POST"])
# @login_required