import pandas as pd
from pandas_datareader import data as web
import plotly.graph_objects as go
stock = 'MSFT'
 
df = web.DataReader(stock, data_source='yahoo', start='01-01-2019')
trace1 = {
    'x': df.index,
    'open': df.Open,
    'close': df.Close,
    'high': df.High,
    'low': df.Low,
    'type': 'candlestick',
    'name': 'MSFT',
    'showlegend': True
}
data = [trace1]
# Config graph layout
layout = go.Layout({
    'title': {
        'text': 'Microsoft(MSFT) Moving Averages',
        'font': {
            'size': 15
        }
    }
})