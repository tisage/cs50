from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *
from datetime import datetime
import time

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
# app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///task.db")


@app.route("/")
@login_required
def index():
    # get all da from database
    openTask = db.execute("SELECT * FROM tasks WHERE userID=:userID and status=:status", userID=session["user_id"],status=0)
    closedTask = db.execute("SELECT * FROM tasks WHERE userID=:userID and status=:status", userID=session["user_id"],status=1)
    return render_template("index.html", openTask=openTask, closedTask=closedTask)
    # return apology("TODO")

@app.route("/add", methods=["GET", "POST"])
@login_required
def add():
    """add a new task"""
    if request.method== "GET":
        return render_template("add.html")
    else:
        # fetch value from form
        taskName = request.form.get("taskName")
        taskCategory = request.form.get("taskCategory")       
        
        priority = int(request.form.get("priority"))
            
        db.execute("INSERT INTO tasks (userId, taskName, category, priority) \
                    VALUES(:userId, :taskName, :category, :priority)", userId=session["user_id"], taskName=taskName, \
                    category=taskCategory, priority=priority)
    
        return redirect(url_for("index"))

@app.route('/delete/<int:taskID>', methods=['GET'])
@login_required
def delete_task(taskID):
    """Deletes task from database"""
    db.execute("DELETE FROM tasks WHERE taskID=:taskID AND userID=:userID", taskID =taskID, userID=session["user_id"])
    return redirect(url_for("index"))


    
@app.route('/complete/<int:taskID>', methods=['GET'])
@login_required
def complete_task(taskID):
    """complete task from database"""
    
    # use time library to store current Date and Time with SQL format
    currentTime= time.strftime('%Y-%m-%d %H:%M:%S')
    # update tasks database
    db.execute("UPDATE tasks SET endTime=:endTime, status=:status WHERE taskID=:taskID AND userID=:userID", taskID=taskID, userID=session["user_id"], endTime=currentTime, status=1)
    # calculate time spent
    
    return redirect(url_for("index"))    


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
        
        # create username in database
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", \
            username = request.form.get("username"), hash = pwd_context.hash(request.form.get("password")))
        
        if not result:
            return apology("username already exist")
            
        # remember which user has logged in
        session["user_id"] = result

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")
