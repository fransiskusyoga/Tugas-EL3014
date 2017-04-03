//Pin Assignment
int segment[] ={5,A5,A4,2,3,4,A2,A3};
int seg_con[]  = {6,7,8,9};
int INPUT1=11, INPUT2=12;
int LEDdetik=10;
int buzzer=A1;

//variabel waktu
signed char detik=0,menit=0,jam=0;
int halfsecond=0;

//variabel timer
int timer=0;
int timerStat=0;

//variabel input
int inputStatus1=1, inputStatus2=1; //active low
uint32_t debounce1, debounce2;
int debounceTime=200; //milisecond

//Variable kontrol
int i1, i2;
int temp;
int mode=0;

//Fungsi dan prosedur
void tampil_7(int nilai);
void show(int b, int a);

void setup() 
{  
  // Pengesetan pin output untuk 7-segmen
  for (int i=0; i<8; i++) pinMode (segment[i],OUTPUT);

  // pengesetan output control pada 7-segment
  for (int i=0;i<4;i++) pinMode (seg_con[i], OUTPUT);    

  //pengesetan pin input dengan pull up (active low)
  pinMode(INPUT1, INPUT); digitalWrite(INPUT1, HIGH);
  pinMode(INPUT2, INPUT); digitalWrite(INPUT2, HIGH);

  //pengesetan pin lainnya
  pinMode(LEDdetik, OUTPUT);
  pinMode(buzzer, OUTPUT); digitalWrite(buzzer, HIGH);
  
  // Stop interrupts
  cli();
  
  // Initialize Timer/Counter Register
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; //initialize counter value to 0

  // Turn on CTC mode
  TCCR1B |= (1 << WGM12);
  
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  
  // a value of 15624 in a second. By having OCR1A = 15624 / 2 then the interrupt function
  OCR1A = 7812;
  
  // Enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  // Allow interrupts
  sei();

  //seting anti debounce
  debounce1=millis();
  debounce2=millis();
  Serial.begin(9600);
}

void loop() { 
  //Menerima input
  inputStatus1=1;
  inputStatus2=1;
  if(digitalRead(INPUT1)==0)
  {
      if (millis()-debounce1>debounceTime)
      {
        debounce1=millis();
        inputStatus1=0;
      }
  }
  if(digitalRead(INPUT2)==0)
  {
      if (millis()-debounce2>debounceTime)
      {
        debounce2=millis();
        inputStatus2=0;
      }
  }

  //debuiging print
  /*Serial.print("INPUT1 = ");
  Serial.println(inputStatus1);
  Serial.print("INPUT2 = ");
  Serial.println(inputStatus2);*/

  //Main program
  switch (mode){
    //mode jam
    case 0:
      {
        //Inisiasi
        i1=1; i2=0; temp=0;
        
        //LED Blinking
        if (halfsecond) digitalWrite(LEDdetik,HIGH);
        else digitalWrite(LEDdetik,LOW);

        //Mode Jump
        if (inputStatus1==0) mode=10; //set jam
        if (inputStatus2==0) mode=20; //set alarm

        //Alarm check
        if (timerStat==1 && timer<0)
        {
          digitalWrite(buzzer, LOW);
          if (inputStatus2==0)
          {
            mode=0;
            timerStat=0;
            digitalWrite(buzzer, HIGH);
          }
        }
        
        //output
        show(jam,menit);
      }
      break;
      
    //mode pengesetan jam
    case 10:
      {
        if (inputStatus1==0)
        {
          temp+=i2*i1;
          i2=0; i1*=10;
          if(i1==10000)
          {
            jam=temp/100;
            menit=temp%100;
            i1=1; i2=0; temp=0;
            mode=0;
          }
        }
        if (inputStatus2==0)
        {
          i2++;
          if(i2>9) i2=0;
        }
        show((temp+i2*i1)/100, (temp+i2*i1)%100);
      }
      break;
      
    //mode timer
    case 20:
      {
        if (inputStatus1==0)
        {
          temp+=i2*i1;
          i2=0; i1*=10;
          if(i1==10000)
          {
            timer=(temp/100)*timer%100;
            i1=1; i2=0; temp=0;
            mode=0;
            timerStat=1;
            //Serial.print("detik"); Serial.println(timer);
          }
        }
        if (inputStatus2==0)
        {
          i2++;
          if(i2>9) i2=0;
        }
        show((temp+i2*i1)/100, (temp+i2*i1)%100);
      }
      break;
  }
}

//Interupt
ISR(TIMER1_COMPA_vect){  
  halfsecond++;
  if(halfsecond>1)
  {
    halfsecond=0;
    detik++;
    timer--;
  }
  if(detik>59)
  {
      detik= 0;
      menit++;
  }
  if(menit>59)
  {
      menit= 0;
      jam++;
  }
  if(jam>23)
  {
      jam=0;
  }
}

// Menampilakn angka pada 7-segment
void tampil_7(int nilai) 
{
  if (nilai==0){
            digitalWrite(segment[0], HIGH);
            digitalWrite(segment[1], HIGH);
            digitalWrite(segment[2], LOW); 
            digitalWrite(segment[3], LOW);
            digitalWrite(segment[4], LOW);      
            digitalWrite(segment[5], LOW);
            digitalWrite(segment[6], LOW);
            digitalWrite(segment[7], LOW);
  }
  else if (nilai==1) {
            digitalWrite(segment[0], HIGH);
            digitalWrite(segment[1], HIGH);
            digitalWrite(segment[2], HIGH);
            digitalWrite(segment[3], HIGH);
            digitalWrite(segment[4], HIGH);
            digitalWrite(segment[5], LOW);
            digitalWrite(segment[6], LOW);
            digitalWrite(segment[7], HIGH);   
  }
  else if (nilai==2){
            digitalWrite(segment[0], HIGH);
            digitalWrite(segment[1], LOW);
            digitalWrite(segment[2], HIGH);
            digitalWrite(segment[3], LOW);
            digitalWrite(segment[4], LOW);
            digitalWrite(segment[5], HIGH);
            digitalWrite(segment[6], LOW);
            digitalWrite(segment[7], LOW);
  }
  else if (nilai==3)  {
          digitalWrite(segment[0], HIGH);
          digitalWrite(segment[1], LOW);
          digitalWrite(segment[2], HIGH);
          digitalWrite(segment[3], HIGH);
          digitalWrite(segment[4], LOW);
          digitalWrite(segment[5], LOW);
          digitalWrite(segment[6], LOW);
          digitalWrite(segment[7], LOW);
  }
  else if (nilai==4){
            digitalWrite(segment[0], HIGH);
            digitalWrite(segment[1], LOW);
            digitalWrite(segment[2], LOW);
            digitalWrite(segment[3], HIGH);
            digitalWrite(segment[4], HIGH);
            digitalWrite(segment[5], LOW);
            digitalWrite(segment[6], LOW);
            digitalWrite(segment[7], HIGH);
  }
  else if (nilai==5){
            digitalWrite(segment[0], HIGH);
            digitalWrite(segment[1], LOW);
            digitalWrite(segment[2], LOW);
            digitalWrite(segment[3], HIGH);
            digitalWrite(segment[4], LOW);
            digitalWrite(segment[5], LOW);
            digitalWrite(segment[6], HIGH);
            digitalWrite(segment[7], LOW);
  }
  else if (nilai==6){
            digitalWrite(segment[0], HIGH);
            digitalWrite(segment[1], LOW);
            digitalWrite(segment[2], LOW);
            digitalWrite(segment[3], LOW);
            digitalWrite(segment[4], LOW);
            digitalWrite(segment[5], LOW);
            digitalWrite(segment[6], HIGH);
            digitalWrite(segment[7], LOW);
  }
  else if (nilai==7){
            digitalWrite(segment[0], HIGH);
            digitalWrite(segment[1], HIGH);
            digitalWrite(segment[2], HIGH);
            digitalWrite(segment[3], HIGH);
            digitalWrite(segment[4], HIGH);
            digitalWrite(segment[5], LOW);
            digitalWrite(segment[6], LOW);
            digitalWrite(segment[7], LOW);
  }
  else if (nilai==8){
            digitalWrite(segment[0], HIGH);
            digitalWrite(segment[1], LOW);
            digitalWrite(segment[2], LOW);
            digitalWrite(segment[3], LOW);
            digitalWrite(segment[4], LOW);
            digitalWrite(segment[5], LOW);
            digitalWrite(segment[6], LOW);
            digitalWrite(segment[7], LOW);
  }
  else{
            digitalWrite(segment[0], HIGH);
            digitalWrite(segment[1], LOW);
            digitalWrite(segment[2], LOW);
            digitalWrite(segment[3], HIGH);
            digitalWrite(segment[4], LOW);
            digitalWrite(segment[5], LOW);
            digitalWrite(segment[6], LOW);
            digitalWrite(segment[7], LOW);
  }
}

// menampilkan jam digital
void show(int b, int a)
{
    digitalWrite(seg_con[3],HIGH);
    digitalWrite(seg_con[2],LOW);
    digitalWrite(seg_con[1],LOW);
    digitalWrite(seg_con[0],LOW);
    tampil_7(a%10);
    delay(5);
    
    digitalWrite(seg_con[3],LOW);
    digitalWrite(seg_con[2],HIGH);
    digitalWrite(seg_con[1],LOW);
    digitalWrite(seg_con[0],LOW);
    tampil_7(a/10);
    delay(5);
   
    digitalWrite(seg_con[3],LOW);
    digitalWrite(seg_con[2],LOW);
    digitalWrite(seg_con[1],HIGH);
    digitalWrite(seg_con[0],LOW);
    tampil_7(b%10);
    delay(5);
    
    digitalWrite(seg_con[3],LOW);
    digitalWrite(seg_con[2],LOW);
    digitalWrite(seg_con[1],LOW);
    digitalWrite(seg_con[0],HIGH);
    tampil_7(b/10);
    delay(5);
}

