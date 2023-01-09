from dash import Dash, html, dcc, Input, Output
import dash_daq as daq
import datetime as dt
import plotly.graph_objects as go

# can-bus raw data
raw_accel = [0.5, 1.2, 9.8]
raw_gyro = [0.2, 0.4, 0.6]

# plotly dash
app = Dash(__name__)

colors = {
    'background': '#111111',
    'text': '#7FDBFF'
}

app.layout = html.Div(style={'backgroundColor': colors['background']}, children=[
    html.H1(
        children='CAN-Bus',
        style={
            'textAlign': 'center',
            'color': colors['text']
        }
    ),
    daq.LEDDisplay(
        id='LED-accelX',
        label="X-accel",
        value=raw_accel[0],
        color="#FF5E5E",
        backgroundColor=colors['background']
    ),
    daq.LEDDisplay(
        id='LED-time',
        label="Time",
        value="00:00:00",
        color="#FF5E5E",
        backgroundColor=colors['background']
    ),
    dcc.Interval(
        id='interval-component',
        interval=100, # in milliseconds
        n_intervals=0
    )
])

@app.callback(
    Output('LED-time', 'value'),
    Input('interval-component', 'n_intervals'))
def get_current_time(input_value):
    now = dt.datetime.now()
    hours = now.hour
    mins = now.minute
    sec = now.second
    str_time = str(hours) + ":" + str(mins) + ":" + str(sec)
    
    return str_time



if __name__ == '__main__':
    app.run_server(debug=True)