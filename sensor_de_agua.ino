int SENSOR;
#DEFINE LED 2

void setup () 
{
  pinMode(LED, OUTPUT)
}
void loop ()
{
  SENSOR=analogRead(A0);
  if (SENSOR>0{
    digitalWrite(LED,HIGH);
    }
    else{
      digitalWrite(LED,LOW);    }
}
delay(1000);
}
