import numpy as np
import pandas as pd

import quandl

ticker = 'AAPL'
startDate='2017-01-01'
endDate = '2017-11-05'

# replace with your own api_key, register from quandl api website
quandl.ApiConfig.api_key ="gw5WD-q5-b9vT2coXsL8"
data = quandl.get_table("WIKI/PRICES", qopts={'columns': ['ticker', 'date', 'volume', 'close']}, \
                                ticker=[ticker], date={'gte': startDate, 'lte': endDate})
                            
print(data.head(10))
