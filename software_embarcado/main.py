import paho.mqtt.client as mqtt
import sys
import json
from random import randrange

#definicoes: 
Broker = "localhost"
PortaBroker = 1883
KeepAliveBroker = 60
TopicoSubscribe = "apply-medication" 
def apply_medication(data):
    #Rotina de preparação de doses
    #-----------------------------
    resposta={}
    resposta["medicationId"] = data["medicationId"]
    resposta["nurseId"]=data["nurseId"]
    resposta["cupId"] = randrange(6)
    resp = json.dumps(resposta)

    client.publish("medication-response",resp)
#Callback - conexao ao broker realizada
def on_connect(client, userdata, flags, rc):
    print("[STATUS] Conectado ao Broker. Resultado de conexao: "+str(rc))
    client.subscribe(TopicoSubscribe)

#Callback - mensagem recebida do broker
def on_message(client, userdata, msg):
    mensagem = json.loads(msg.payload)
    apply_medication(mensagem)
    
    
	

#programa principal:
try:
        print("[STATUS] Inicializando MQTT...")
        #inicializa MQTT:
        client = mqtt.Client()
        client.on_connect = on_connect
        client.on_message = on_message

        client.connect(Broker, PortaBroker, KeepAliveBroker)
        client.loop_forever()
except KeyboardInterrupt:
        print ("\nCtrl+C pressionado, encerrando aplicacao e saindo...")
        sys.exit(0)