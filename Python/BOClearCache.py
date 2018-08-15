import os
import sys
import time
import shutil
import logging
from socket import *
import string
from datetime import date

log = "BOClearCache.log"
logger = logging.getLogger("BOCacheClear_Reboot_1")
FORMAT = "%(asctime)s - %(name)s - %(levelname)s:%(message)s"
logging.basicConfig(filename=log,format = FORMAT, level = logging.DEBUG)


def main():
    logger.info('\n\n---------------------------------------------START BO CACHE CLEAR------------------------------------------------------\n')

    #Send MM email
    sender = input("What is your email address?")
    ticketNum = input("What is the ticket number?")
    logger.debug("Sending an email to place CO in Maintenance Mode")
    email(sender, ticketNum)
    wait = input("PRESS ENTER TO CONTINUE.")

    #Stop BO services
    logger.debug("Stopping BO services on BO Server")

    #Stop Apache Tomcat 6.0
    logger.debug("Stopping Apache Tomcat 6.0 Service at location D: busapps\programs\Business Objects\Tomcat6\bin\Tomcat6.exe")
    changeDir("D:/busapps/programs/Business Objects/Tomcat6/bin")
    os.system("Tomcat6.exe stop")
    time.sleep(20)
    logger.debug("Checking if Apache Tomcat 6.0 was stopped")
    statusTomcat = checkStatusStopped("Tomcat6")
    countTomcat = 0
    while not statusTomcat:
        print("Apache Tomcat was not stopped or is still stopping")
        time.sleep(40)
        os.system("Tomcat6.exe stop")
        time.sleep(20)
        statusTomcat = checkStatusStopped("Tomcat6")
        countTomcat += 1
        if statusTomcat:
            break
        if countTomcat >= 5:
            logger.debug("Could not stop Apache Tomcat 6.0. Stopping the automation script")
            sys.exit()

    #Stop Server Intelligence Agent    
    logger.debug("Stopping Server Intelligence Agent Service at location D: busapps\programs\Business Objects\BusinessObjects Enterprise 12.0\win32_x86\sia.exe")
    changeDir("D:/busapps/programs/Business Objects/BusinessObjects Enterprise 12.0/win32_x86") 
    os.system("stopSIA.bat")
    time.sleep(20)
    logger.debug("Checking if Server Intelligence Agent Service was stopped")
    statusSIA = checkStatusStopped("BOE120SIAUSILASP00208 ")
    countSIA = 0
    while not statusSIA:
        print("Server Intelligence Agent was not stopped or is still stopping")
        time.sleep(40)
        os.system("stopSIA.bat")
        time.sleep(20)
        statusSIA = checkStatusStopped("BOE120SIAUSILASP00208 ")
        countSIA += 1
        if statusSIA:
            break
        if countSIA >= 5:
            logger.debug("Could not stop Server Intelligence Agent Service. Stopping the automation script")
            sys.exit()


    path = "D:/Automation/IBMoutput.txt"


    #Stops IBM Websphere Application Server – taserver01
    logger.debug("Stopping IBM Websphere Application Server – taserver01")
    changeDir("D:/busapps/programs/IBM/WebSphere/AppServer/bin")
    os.system("stopServer.bat taserver01")
    time.sleep(15)
    logger.debug("Checking if IBM Websphere Application Server – taserver01 was stopped")
    statusTA = False
    os.system("serverStatus.bat taserver01 > " + path)
    time.sleep(10)
    file = open(path)
    for line in file:
        if """The Application Server "taserver01" cannot be reached.""" in line:
            statusTA = True
            logger.debug("Server taserver01 stopped")
            break
    file.close()
    countTA = 0
    while not statusTA:
        print("Taserver01 was not stopped or is still stopping")
        time.sleep(40)
        os.system("stopServer.bat taserver01")
        time.sleep(20)
        os.system("serverStatus.bat taserver01 > " + path)
        file = open(path)
        for line in file:
            if """The Application Server "taserver01" cannot be reached.""" in line:
                statusTA = True
                logger.debug("Server taserver01 stopped")
                break
        file.close()
        countTA += 1
        if statusTA:
            break
        if countTA >= 5:
            logger.debug("Could not stop IBM Websphere Application Server – taserver01. Stopping the automation script")
            sys.exit()

    #Stops IBM WebSphere Application Server-Nodeagent 
    logger.debug("Stopping IBM WebSphere Application Server V7.0 - ASP00208-TANode01-Nodeagent Service")
    os.system("stopServer.bat nodeagent")
    time.sleep(15)
    logger.debug("Checking if IBM WebSphere Application Server V7.0 - ASP00208-TANode01-Nodeagent was stopped")
    os.system("serverStatus.bat nodeagent > " + path)
    time.sleep(10)
    file = open(path)
    statusNode = False
    for line in file:
        if """The Node Agent "nodeagent" cannot be reached.""" in line:
            statusNode = True
            logger.debug("Server nodeagent stopped")
            break
    file.close()
    countNode = 0
    while not statusNode:
        print("Nodeagent was not stopped or is still stopping")
        time.sleep(40)
        os.system("stopServer.bat nodeagent")
        time.sleep(20)
        os.system("serverStatus.bat nodeagent > " + path)
        file = open(path)
        for line in file:
            if """The Node Agent "nodeagent" cannot be reached.""" in line:
                statusNode = True
                logger.debug("Server nodeagent stopped")
                break
        file.close()
        countNode += 1
        if statusNode:
            break
        if countNode >= 5:
            logger.debug("Could not stop IBM WebSphere Application Server V7.0 - ASP00208-TANode01-Nodeagent . Stopping the automation script")
            sys.exit()
    
    #Renames folder
    logger.debug("Renaming the Data folder: Data_mmddyy")
    changeDir("D:/busapps/programs/Business Objects/BusinessObjects Enterprise 12.0")
    renameDir()

    #Clears cache
    logger.debug("Delete contents of Temp folder in Tomcat at location D:\busapps\programs\Business Objects\Tomcat6\temp")
    clearCache("D:/busapps/programs/Business Objects/Tomcat6/temp")
    logger.debug("Delete contents of Work folder in Tomcat at location D:\busapps\programs\Business Objects\Tomcat6\work")
    clearCache("D:/busapps/programs/Business Objects/Tomcat6/work")

    #Starts BO services
    logger.debug("Starting BO services")
    
    #Starts Apache Tomcat 6.0
    logger.debug("Starting Apache Tomcat 6.0 Service at location D:busapps\programs\Business Objects\Tomcat6\bin\Tomcat6.exe")
    changeDir("D:/busapps/programs/Business Objects/Tomcat6/bin")
    os.system("Tomcat6.exe start")
    time.sleep(20)
    logger.debug("Checking if Apache Tomcat 6.0 started")
    statusTomcat1 = checkStatusRunning("Tomcat6")
    countTomcat1 = 0
    while not statusTomcat1:
        time.sleep(40)
        os.system("Tomcat6.exe start")
        time.sleep(20)
        statusTomcat1 = checkStatusRunning("Tomcat6")
        countTomcat1 += 1
        if statusTomcat1:
            break
        if countTomcat1 >= 5:
            logger.debug("Could not start Apache Tomcat 6.0. Stopping the automation script")
            sys.exit()
    
    #Starts Server Intelligence Agent       
    logger.debug("Starting Server Intelligence Agent Service at location D:\busapps\programs\Business Objects\BusinessObjects Enterprise 12.0\win32_x86\sia.exe")
    changeDir("D:/busapps/programs/Business Objects/BusinessObjects Enterprise 12.0/win32_x86")
    os.system("startSIA.bat")
    time.sleep(20)
    logger.debug("Checking if Server Intelligence Agent Service started")
    statusSIA1 = checkStatusRunning("BOE120SIAUSILASP00208 ")
    countSIA1 = 0
    while not statusSIA1:
        time.sleep(40)
        os.system("startSIA.bat")
        time.sleep(20)
        statusSIA1 = checkStatusRunning("BOE120SIAUSILASP00208 ")
        countSIA1 += 1
        if statusSIA1:
            break
        if countSIA1 >= 5:
            logger.debug("Could not start Server Intelligence Agent Service. Stopping the automation script")
            sys.exit()

    #Starts IBM Websphere Application Server - Nodeagent   
    logger.debug("Starting IBM WebSphere Application Server V7.0 - ASP00208-TANode01-Nodeagent Service")
    changeDir("D:/busapps/programs/IBM/WebSphere/AppServer/bin")
    os.system("startServer.bat nodeagent")
    time.sleep(15)
    logger.debug("Checking if IBM WebSphere Application Server V7.0 - ASP00208-TANode01-Nodeagent was started")
    os.system("serverStatus.bat nodeagent > " + path)
    statusNode1 = False
    time.sleep(10)
    file = open(path)
    for line in file:
        if """The Node Agent "nodeagent" is STARTED""" in line:
            statusNode1 = True
            logger.debug("Server nodeagent open for e-business")
            break
    file.close()
    countNode1 = 0
    while not statusNode1:
        time.sleep(40)
        os.system("startServer.bat nodeagent")
        time.sleep(30)
        os.system("serverStatus.bat nodeagent > " + path)
        file = open(path)
        for line in file:
            if """The Node Agent "nodeagent" is STARTED""" in line:
                statusNode1 = True
                logger.debug("Server nodeagent open for e-business")
                break
        file.close()
        countNode1 += 1
        if statusNode1:
            break
        if countNode1 >= 5:
            logger.debug("Could not start IBM WebSphere Application Server V7.0 - ASP00208-TANode01-Nodeagent . Stopping the automation script")
            sys.exit()

    #Starts IBM Websphere Application Server – taserver01
    logger.debug("Starting IBM Websphere Application Server – taserver01")
    os.system("startServer.bat taserver01")
    time.sleep(15)
    logger.debug("Checking if IBM Websphere Application Server – taserver01 was started")
    os.system("serverStatus.bat taserver01 > " + path)
    time.sleep(10)
    file = open(path)
    statusTA1 = False
    for line in file:
        if """The Application Server "taserver01" is STARTED""" in line:
            statusTA1 = True
            logger.debug("Server taserver01 open for e-business")
            break
    file.close()
    countTA1 = 0
    while not statusTA1:
        time.sleep(40)
        os.system("startServer.bat taserver01")
        time.sleep(30)
        os.system("serverStatus.bat taserver01 > " + path)
        file = open(path)
        for line in file:
            if """The Application Server "taserver01" is STARTED""" in line:
                statusTA1 = True
                logger.debug("Server taserver01 open for e-business")
                break
        file.close()
        countTA1 += 1
        if statusTA1:
            break
        if countTA1 >= 5:
            logger.debug("Could not start IBM Websphere Application Server – taserver01. Stopping the automation script")
            sys.exit() 
    wait = input("PRESS ENTER TO CONTINUE.")

    #Send validation email 
    logger.debug("Sending a validation email\n")
    email(sender,  ticketNum, True)

    logger.info('\n---------------------------------------------END BO CACHE CLEAR------------------------------------------------------\n\n')


    
#Changes directories
def changeDir(path):
    os.chdir(path)
    
#Renames the Data folder
def renameDir():
    newName = "Data_" 
    if date.today().month < 10:
        newName += "0" + str(date.today().month)
    else:
        newName += str(date.today().month)
    if date.today().day < 10:
       newName += "0" + str(date.today().day)
    else:
        newName += str(date.today().day)
    newName += str(date.today().year)
    print(newName)
    if os.path.exists("D:/busapps/programs/Business Objects/BusinessObjects Enterprise 12.0/Data"):
        if os.path.exists("D:/busapps/programs/Business Objects/BusinessObjects Enterprise 12.0/" + newName) :
            logger.debug("Data folder " + newName + "already exists. Renaming the folders.")
            os.rename(newName, newName + "_old")
        try:
            os.rename("Data", newName)
        except Exception as e:
            logger.debug(e)
            restart = input("The Data folder was not able to be renamed. Press ENTER to stop CompData and continue to reboot the system. Otherwise exit the program.")
            logger.info('\n---------------------------------------------REBOOT BO CACHE CLEAR------------------------------------------------------\n')
            logger.debug("Stopping CompData Query Tool for reboot")
            os.system("net STOP CompData")
            time.sleep(20)
            os.system("sc query CompData > output.txt")
            time.sleep(15)
            file = open("output.txt")
            for line in file:
                    if "STOPPED" in line:
                        logger.debug("CompData was stopped")
                        logger.debug("Restarting system since the Data folder could not be renamed\n")
                        file.close()
                        os.system("shutdown /r")
            print("CompData was not stopped")
            logger.debug("CompData was not stopped. Stopping script. Please stop manually.")
            sys.exit()
            file.close()
    else:
        return

#Clear contents of the folder
def clearCache(path):
    for file in os.listdir(path):
        file_path = os.path.join(path, file)
        try:
            if os.path.isdir(file_path): 
                shutil.rmtree(file_path)
            elif os.path.isfile(file_path):
                os.unlink(file_path)
        except Exception as e:
            logger.debug(e)

#Checks if the service is running
def checkStatusRunning(serviceName):
    command = "sc query " + serviceName + """| FIND "STATE" | FIND "RUNNING" """ 
    if os.system(command) < 1:
        logger.debug(serviceName + " was started")
        return True
    else:
        logger.debug(serviceName + " was not started or is still starting")
        return False

#Checks if the service is running
def checkStatusStopped(serviceName):
    command = "sc query " + serviceName + """| FIND "STATE" | FIND "STOPPED" """ 
    if os.system(command) < 1:
        logger.debug(serviceName + " was stopped")
        return True
    else:
        logger.debug(serviceName + " was not stopped or is still stopping")
        return False

#Sends an email
def email(sender, ticketNum, validation = False):
    endmsg = b"\r\n.\r\n"
    mailFromCommand = b'MAIL FROM: ' + sender.encode() + b'\r\n'

    #Mail server and port
    mailserver = "mail.ca.com"
    port = 25
    
    if validation == True:
        receiver = "Umesh.Viswanathan@ca.com"
        text = "As per ticket number " + str(ticketNum)+ ", BO cache clear activity has been completed. Please validate."
        msg = b"\r\n " + text.encode()
        subject = b'SUBJECT: Ticket completed and BO cache cleared\r\n'
    else:
        msg = b"\r\nDear Team, \n\nPlease put CO in Maintenance mode (MM) as per ticket number " + ticketNum.encode() + b"."
        receiver = "Team-ITC-NOC@ca.com"
        subject = b'SUBJECT: GCS BO cache clear\r\n'
    

    # Create socket called clientSocket and establish a TCP connection with mailserver
    clientSocket = socket(AF_INET, SOCK_STREAM)
    clientSocket.connect((mailserver, port))
    recv = clientSocket.recv(1024)
    print (recv)
    if recv[:3] != '220':
            print ('220 reply not received from server.')

    # Send HELO command and print server response.
    heloCommand = b'HELO Alice\r\n'
    clientSocket.send(heloCommand)
    recv1 = clientSocket.recv(1024)
    print (recv1)
    if recv1[:3] != '250':
            print ('250 reply not received from server.')

    # Send MAIL FROM command and print server response.
    clientSocket.send(mailFromCommand)
    recv2 = clientSocket.recv(1024)
    print (recv2)

    # Send RCPT TO command and print server response.
    rcptToCommand = b'RCPT TO: ' + receiver.encode() + b'\r\n'
    clientSocket.send(rcptToCommand)
    recv3 = clientSocket.recv(1024)
    print (recv3)

    # Send DATA command and print server response.
    dataCommand = b'DATA\r\n'
    clientSocket.send(dataCommand)
    recv4 = clientSocket.recv(1024)
    print (recv4)

    # Send message data.
    clientSocket.send(subject)
    clientSocket.send(msg)

    # Message ends with a single period.
    clientSocket.send(endmsg)
    recv5 = clientSocket.recv(1024)
    print (recv5)
   
    # Send QUIT command and get server response.
    quitCommand = b'QUIT\r\n'
    clientSocket.send(quitCommand)
    recv6 = clientSocket.recv(1024)
    print (recv6)
    
    

if __name__ == "__main__":
    main()
