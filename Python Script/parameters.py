# ---------------------------------------------- #
# - SCRIPT ATTO A MODIFICARE I PARAMETRI DELLA - #
# - SIMULAZIONE IN MANIERA RAPIDA -------------- #
# ---------------------------------------------- #
# - Dario Figliuzzi ---------------------------- #


pathAirstrip = r'c:/Users/dario/git/airportPE/src/Airstrip.ned'  # contiene sia To che Tl
pathSky = r'c:/Users/dario/git/airportPE/src/Sky.ned'
pathParking = r'c:/Users/dario/git/airportPE/src/Parking.ned'

stringA = 'double procTime2'
stringD = 'double procTime3'
stringSP = 'double procTime'

# ---------{SPECIFICARE QUI I PARAMETRI(sec) PER LA SIMULAZIONE}------------------

newTS_Sky = 'default(48s)'       # T sky
newTS_Landing = 'default(15s)'   # T landing
newTS_Takeoff = 'default(25s)'   # T takeoff
newTS_Parking = 'default(240s)'  # T parking

# -----------------------AIRSTRIP-landing-----------------------------------------

with open(pathAirstrip, 'r') as file:
    filedata = file.readlines()

for i in range(len(filedata)):
    if stringA in filedata[i]:
        index = filedata[i].index('default')
        firstSub = filedata[i][0:index]
        new = str(firstSub) + newTS_Landing + '; //tempo per atterrare\n'
        print(new)
        filedata[i] = new

file.close()

with open(pathAirstrip, 'w') as file:
    for line in filedata:
        file.write(line)

file.close()
# -----------------------AIRSTRIP-takeoff-----------------------------------------

with open(pathAirstrip, 'r') as file:
    filedata = file.readlines()

for i in range(len(filedata)):
    if stringD in filedata[i]:
        index = filedata[i].index('default')
        firstSub = filedata[i][0:index]
        new = str(firstSub) + newTS_Takeoff + '; //tempo per decollare\n'
        print(new)
        filedata[i] = new

file.close()

with open(pathAirstrip, 'w') as file:
    for line in filedata:
        file.write(line)

file.close()

# ------------------------SKY-----------------------------------------------------

with open(pathSky, 'r') as file:
    filedata = file.readlines()

for i in range(len(filedata)):
    if stringSP in filedata[i]:
        index = filedata[i].index('default')
        firstSub = filedata[i][0:index]
        new = str(firstSub) + newTS_Sky + '; //interarrival time\n'
        print(new)
        filedata[i] = new

file.close()

with open(pathSky, 'w') as file:
    for line in filedata:
        file.write(line)

file.close()

# -------------------------PARKING------------------------------------------------

with open(pathParking, 'r') as file:
    filedata = file.readlines()

for i in range(len(filedata)):
    if stringSP in filedata[i]:
        index = filedata[i].index('default')
        firstSub = filedata[i][0:index]
        new = str(firstSub) + newTS_Parking + '; //tempo in cui rimane parcheggiato\n'
        print(new)
        filedata[i] = new

file.close()

with open(pathParking, 'w') as file:
    for line in filedata:
        file.write(line)

file.close()

