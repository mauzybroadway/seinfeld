
#include <Wire.h> //I2C Arduino Library

#define address 0x1E //0011110b, I2C 7bit address of HMC5883

//#define CEREAL

int led = 13;

bool check_magno();
void read_magno(int *x, int *y, int *z);
void play_song();

void setup(){
  
  //Initialize Serial and I2C communications
  //#ifdef CEREAL
  Serial.begin(9600);
  //#endif
  
  Wire.begin();
  
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
}

void loop(){
  if( check_magno() ) {
    play_song();
  }

  delay(200);
}

void play_song() {
  Serial.println("PLAYING SONG");
}

void min_max(int *arr, int len, int *min_idx, int *max_idx) {
  int i = 0, minn = 1000, maxx = -1000;
  
  for(i = 0; i < len; i++) {
    if(arr[i] < minn){
      *min_idx = i;
      minn = arr[i];
    }

    if(arr[i] > maxx){
      *max_idx = i;
      maxx = arr[i];
    }
  }
}

bool check_magno() {

  static int xx[10], yy[10], zz[10];
  static int idx;
  static bool queue_ready = false;
  
  int x_min, x_max, y_min, y_max, z_min, z_max;
  
  read_magno(&xx[idx], &yy[idx], &zz[idx]);
  xx[idx] *= -1; // GENERALIZE THIS

  idx = (idx + 1) % 10;
  
  if(queue_ready) {
    min_max(xx, 10, &x_min, &x_max);
    min_max(zz, 10, &z_min, &z_max);

    //#ifdef CEREAL
    Serial.print(xx[x_min]);
    Serial.print(",");
    Serial.print(xx[x_max]);
    Serial.print(",");
    Serial.print(zz[x_min]);
    Serial.print(",");
    Serial.println(zz[x_max]);
    //#endif

    if (xx[x_max] - xx[x_min] >= 100 &&
	zz[z_max] - zz[z_min] >= 100 &&
	x_max < idx && x_min >= 10 - idx &&
	z_max < idx && z_max >= 10 - idx) {
      queue_ready = false;
      idx = 0;
      return true;
    }
  } else if (idx == 0) {
    queue_ready = true;
  }
  
  return false;
}

void read_magno(int *x, int *y, int *z) {
  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(address);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
  
  //Read data from each axis, 2 registers per axis
  Wire.requestFrom(address, 6);
  if(6<=Wire.available()){
    *x = Wire.read()<<8; //X msb
    *x |= Wire.read(); //X lsb
    *z = Wire.read()<<8; //Z msb
    *z |= Wire.read(); //Z lsb
    *y = Wire.read()<<8; //Y msb
    *y |= Wire.read(); //Y lsb
  }
}
