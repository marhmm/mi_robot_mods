import json, requests, time, subprocess, schedule, time
from requests.exceptions import ConnectionError


def start():
	print('Turn on!')
	subprocess.call(["433Utils/RPi_utils/steuerung", "1"])

# ************* CLEANING SCHEDULE ************
schedule.every().tuesday.at("15:00").do(start)
schedule.every().friday.at("15:00").do(start)
# ********************************************

counter=0
while 1:	
	time.sleep(60)
	schedule.run_pending()
	try:
		# get current state of robot (ENTER YOUR OWN VALETUDO IP)
		ret=requests.get('http://192.168.178.36/api/current_status')
		output=json.loads(ret.text)	
		state=output['state']
		
		if state==8:
			counter=counter+1
			print('charging...')
		else:
			counter=0
		
		if counter >= 3:		# 3 min			
			if output['battery'] >= 80:
				print('Shutdown!')
				subprocess.call(["433Utils/RPi_utils/steuerung", "0"])
				
				#time.sleep(5)
				# shutdown over ESP8266, not needed with auto shutdown
				#requests.get('http://YYY.YYY.YYY.YYY/shutdown')
			else:
				print('Wait, Batt charge only: ', output['battery'])

	except ConnectionError:
		pass
		#print('offline')

