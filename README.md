# CS 122A - Air Quality Meter Station

# Project Information: 
<!--- The project already has a “CS100 RShell” header. Under that you should have the quarter and year as well as both partners names and SID--->
* Fall 2019
* James Moron

# Introduction: 
<!--- Start by giving a brief overview of what your program is able to accomplish (or what it will be able to accomplish after it is finished in this case) as well as any design patterns that are used and a brief overview of how the inputs are ingested, transformed into classes and structures, and processed.--->

Building an air quality weather station can tell us a lot of what is going on with the environment, such as sudden changes in CO2  and temperature can alert us if there's a fire happening in remote areas without having constant human supervision. Living in southern California, we have yearly advisories for wildfires due to lack of rain, I believe that if we can keep track of the quality of air with an embedded system, we can use this device as a first alert system and reduce chances of the spread of wildfires in California. I live in the Coachella Valley where we are affected greatly by the smells that come out of the Salton Sea throughout the year. This odor that is smelled throughout the whole valley contain trace particles of decaying matter from the man made body of water, with a station deployed at the Salton Sea we can keep track of changes in the air to further understand how this particulate matter may affect us in the long run. With sensors, the atmega 1284, and a raspberry pi w zero we can make a simple station. 

In simple terms, this project is a data collecting server with a Carbon Monoxide sensor, CO2 sensor, and Air Quality Control sensor, along with a relative temperature/humidity sensor. Each of these sensors will be an input to the atmega1284 and be collected on the raspberry pi which will be updated to a database and viewable on your cell phone. When air quality conditions reach unsatisfactory conditions the system will email you an alert.


# Diagram: 
<!--- Create an OMT diagram (or a small number of separate OMT diagrams depending on how complex) that shows the classes in your design and how they interact. You must use a drawing program to create your OMT diagram. OMT diagrams that are drawn by hand and scanned/photographed will not be allowed. Websites and online applications such as Lucidchart and Google Docs as well as programs such as GIMP and Inkscape are capable of creating these diagrams. You should add the images to the images/ folder in your assignment repository which you can then embed in your README.md using markdown.---> 

![Hardware Diagram](/images/image2.png)
![Wiring Diagram](/images/image1.png)
![Wiring Image](/images/wiring.jpg)
![Wiring Image](/images/20191211_185655.jpg)


