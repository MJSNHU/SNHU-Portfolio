from dash import Dash
# Dash 2.11 and later supports running Dash apps in classic Jupyter Notebooks and in JupyterLab without 
# the need to update the code or use the additional JupyterDash library. 
# https://dash.plotly.com/dash-in-jupyter

# Setup the Jupyter version of Dash
# from jupyter_plotly_dash import JupyterDash

# Configure the necessary Python module imports
import dash_leaflet as dl
from dash import dcc, ctx
from dash import html
import plotly.express as px
import plotly.graph_objects as go
from dash import dash_table
from dash import Input, Output, State


# Configure the plotting routines
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


#### FIX ME #####
# change animal_shelter and AnimalShelter to match your CRUD Python module file name and class name
from CRUD import AnimalShelter


###########################
# Data Manipulation / Model
###########################
# FIX ME update with your username and password and CRUD Python module name. NOTE: You will
# likely need more variables for your constructor to handle the hostname and port of the MongoDB
# server, and the database and collection names

username = "aacuser"
password = "SNHU1234"
shelter = AnimalShelter(username, password)


# class read method must support return of list object and accept projection json input
# sending the read method an empty document requests all documents be returned
df = pd.DataFrame.from_records(shelter.read({}))

# MongoDB v5+ is going to return the '_id' column and that is going to have an 
# invlaid object type of 'ObjectID' - which will cause the data_table to crash - so we remove
# it in the dataframe here. The df.drop command allows us to drop the column. If we do not set
# inplace=True - it will reeturn a new dataframe that does not contain the dropped column(s)
df.drop(columns=['_id'],inplace=True)

## Debug
# print(len(df.to_dict(orient='records')))
# print(df.columns)

def getFigure(df):
    breeds = df[df.columns[5]].unique()
    values = []
    for breed in breeds:
        values.append(df[df['breed'] == breed].breed.count())
    fig = go.Figure(px.pie(df, 
                           values=values,
                           names=breeds,
                           title="Avaliable Breeds", 
                          ))
    fig.update_traces(textposition='inside', textinfo='none',
                 hovertemplate = "%{label}: <br>%{value}")
    
    return fig

#########################
# Dashboard Layout / View
#########################
app = Dash('SimpleExample')

getRowStyle = {
    "styleConditions": [
        {
            "condition": "params.rowIndex % 2 === 0",
            "style": {"backgroundColor": "grey", "color": "white"},
        },
    ]
}

app.layout = html.Div([
    html.Div(id='hidden-div', style={'display':'%{label}'}),
    html.A([
        html.Img(
          src='assets/GraziosoSalvareLogo.png',
          style={
              'height' : '10%',
              'width' : '10%',
              'float' : 'right',
              'position' : 'relative',
              'padding-top' : 0,
              'padding-right' : 0})
    ],
    href='https://snhu.edu', target='none'),
    html.Center(html.B(html.H1('Matt Jackson SNHU CS-340 Dashboard'))),
    html.Hr(),
    html.Div(className='buttonRow',
            children=[
                html.Button(id='btn-water', children="Water Rescue"),
                html.Button(id='btn-mountain',children="Mountain or Wilderness Rescue"),
                html.Button(id='btn-disaster', children="Disaster or Individual Tracking"),
                html.Button(id='btn-reset', children="Reset"),
            ]),
    dash_table.DataTable(
        id='datatable-id',
        columns=[
            {"name": i, "id": i, "deletable": False, "selectable": True} for i in df.columns
        ],
        data=df.to_dict('records'),
        
        selected_rows=[0],
        row_selectable='single',
        page_size=10,
    ),
    html.Br(),
    html.Hr(),
    html.Div([
        html.Div([
            dcc.Graph(id='piechart', style={'display':'None'}),# style={'visibility':'hidden'}), 
        ], style = {'width':'49%','display':'inline-block'}),
        html.Div(
            id='map-id',
            className='col s12 m6',
            style={'width':'49%', 'display':'inline-block'}),
    ], style={'display':'flex'})
])

#############################################
# Interaction Between Components / Controller
#############################################

#This callback will highlight a row on the data table when the user selects it
@app.callback(
    Output('datatable-id', 'style_data_conditional'),
    [Input('datatable-id', 'selected_rows')],
    )

def update_styles(selected_rows):
    return  [{
        'if': { 'row_index' : selected_rows },
        'background_color': '#D2F3FF'
        } for i in selected_rows]

# This callback will select a filter for rescue animal types and update the piechart
@app.callback([Output('datatable-id',"data"),
              Output('piechart','figure'),
              Output('piechart','style')],
              [Input('btn-water', 'n_clicks'),
              Input('btn-mountain', 'n_clicks'),
              Input('btn-disaster', 'n_clicks'),
              Input('btn-reset', 'n_clicks')],
              State('piechart', 'figure')
              )

def on_click(button1, button2, button3, button4, figure):
   
    # if button is Reset, restore all records
    if ctx.triggered_id is None or ctx.triggered_id == "btn-reset":
        df = pd.DataFrame.from_records(shelter.read({}))
        fig = go.Figure()
        onoff={'display':'none'}
    
    elif ctx.triggered_id == 'btn-water':
        df = pd.DataFrame.from_records(shelter.read({
                "breed":{"$in":["Labrador Retriever Mix","Chesapeake Bay Retriever", "Newfoundland"]},
                "sex_upon_outcome":"Intact Female",
                '$and':
                 [
                     {'age_upon_outcome_in_weeks':{'$gte':26}
                     },
                     {'age_upon_outcome_in_weeks':{'$lte':156}
                     }
                 ]
              }))
        fig = getFigure(df)
        onoff={'display':'block'}
          
    elif ctx.triggered_id == 'btn-mountain':
        df = pd.DataFrame.from_records(shelter.read({
                "breed":{"$in":["German Shepherd", "Alaskan Malamute","Old English SheepDog", "Siberian Husky", "Rottweiler"]},
                "sex_upon_outcome":"Intact Male",
                '$and':
                 [
                     {'age_upon_outcome_in_weeks':{'$gte':26}
                     },
                     {'age_upon_outcome_in_weeks':{'$lte':156}
                     }
                 ]
                }))
        fig = getFigure(df)
        onoff={'display':'block'}
        
    elif ctx.triggered_id == "btn-disaster":
        df = pd.DataFrame.from_records(shelter.read({
                "breed":{"$in":["Doberman Pinscher", "German Shepherd", "Golden Retriever", "Bloodhound", "Rottweiler"]},
                "sex_upon_outcome":"Intact Male",
                '$and':
                 [
                     {'age_upon_outcome_in_weeks':{'$gte':20}
                     },
                     {'age_upon_outcome_in_weeks':{'$lte':300}
                     }
                 ]
                }))
        fig = getFigure(df)
        onoff={'display':'block'}
                                                     
    try:         
        df.drop(columns=['_id'],inplace=True)
   
    except Exception as e:
        print(e)
        return [],fig,{'display':'block'}
    #print(fig)
    return df.to_dict('records'),fig, onoff


# This callback will update the geo-location chart for the selected data entry
# derived_virtual_data will be the set of data available from the datatable in the form of 
# a dictionary.
# derived_virtual_selected_rows will be the selected row(s) in the table in the form of
# a list. For this application, we are only permitting single row selection so there is only
# one value in the list.
# The iloc method allows for a row, column notation to pull data from the datatable
@app.callback(
    Output('map-id', "children"),
    [Input('datatable-id', "derived_virtual_data"),
    Input('datatable-id', "derived_virtual_selected_rows")])
def update_map(viewData, index):
#FIXME Add in the code for your geolocation chart
    dff = pd.DataFrame.from_dict(viewData)
    if index is None:
        row = 0
    else:
        row = index[0]
    try:
        return [
            dl.Map(style={'width': '1000px', 'height': '500px'},
                #center=[30.75,-97.48], zoom=10, children=[
                center=[dff.iloc[row,13],dff.iloc[row,14]], zoom=10, children=[
                dl.TileLayer(id="base-layer-id"),
                    dl.Marker(position=[dff.iloc[row,13],dff.iloc[row,14]],
                        children=[
                            dl.Tooltip(dff.iloc[row,4]),
                            dl.Popup([
                                html.H4("Name"),
                                html.P(dff.iloc[row,9])
                            ])
                        ])
                ])
        ]
    except Exception as e:
        # print(e)
        return []

app.run_server(Debug=True,address='127.0.0.1',port=8888)