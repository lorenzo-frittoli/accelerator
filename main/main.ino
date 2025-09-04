#include "Config.h"
#include "RailSwitch.h"

long curr_time;

float cap(float x, float mn, float mx) {
  return min(mx, max(mn, x));
}

struct Coil {
  long start, end;
  int pin;
  Coil();
  Coil(int p) {
    pinMode(p, OUTPUT);
    digitalWrite(p, LOW);
    pin = p;
    start = 0;
    end = 0;
  }
  void tick() {
    digitalWrite(pin, start <= curr_time && curr_time < end);
  }
  void on(long period, float power) {
    // We are already on
    if (curr_time < end) {
      Serial.print("\nCOIL DOUBLE FIRED\n");
      return;
    }
    end = curr_time + period;
    start = end - MAX_COIL_PULSE_MS * cap(power, 0.0, 1.0);
  }
  void onNow(float power) {
    // We are already on
    start = curr_time;
    end = start + MAX_COIL_PULSE_MS * cap(power, 0.0, 1.0);
  }
  void off() {
    // We are already off
    if (curr_time >= end) return;
    // Set the actual length of this pulse so that we will be off exactly this tick
    end = curr_time;
  }
};

int sensor_th = 0;

Coil c[2][3] = {{Coil(12), Coil(11), Coil(10)}, {Coil(9), Coil(8), Coil(7)}};
int sensors[2][3] {{A0, A1, A2}, {A3, A4, A5}};
long last_time[2] = {};
int active_pos[2], curr_pos[2], last_pos[2], mettiicazzodicommenti_pos[2];
bool activate[2];
// period as millis / sector
long period[2];
float pos[2], power[2];

RailSwitch rs(RS_DIR_PIN, RS_STEP_PIN, RS_ENABLE_PIN, RS_NUMBER_OF_STEPS, RS_MAX_SPEED, RS_MOVE_SPEED);
long pallaggio_started_time = -1;
bool is_first_spinnaggio = false;

void reset() {
  for (int i = 0; i < COILS; i++) for (int j = 0; j < 3; j++) {
    c[i][j].off();
    c[i][j].tick();
  }
  if (STOP_AFTER_FIRST_LOOP) {
    if (is_first_spinnaggio) while (true);
    else is_first_spinnaggio = true;
  }
  active_pos[0] = -1;
  active_pos[1] = -1;
  curr_pos[0] = -1;
  curr_pos[1] = -1;
  last_pos[0] = -1;
  last_pos[1] = -1;
  period[0] = 0;
  period[1] = 0;
  last_time[0] = 0;
  last_time[1] = 0;
  mettiicazzodicommenti_pos[0] = -1;
  mettiicazzodicommenti_pos[1] = -1;
  rs.openChamber();
  pallaggio_started_time = -1;
}

int next(int i) {
  return i == 2 ? 0 : i + 1;
}

void setup() {
  delay(1000);

  curr_time = (long)millis();

  Serial.begin(115200);
  reset();

  // Calibrate sensors
  for (int i = 0; i < COILS; i++) {
    for (int s = 0; s < 3; s++) pinMode(sensors[i][s], INPUT);
  }

  int rail_th[2];
  for (int i = 0; i < 1; i++) {
    int th[3] = {0, 0, 0};
    for (int rep = 0; rep < CALIB_ROUNDS; rep++) {
      for (int s = 0; s < 3; s++) {
        th[s] = max(th[s], analogRead(sensors[i][s]));
      }
    }
    rail_th[i] = th[0] + th[1] + th[2] - max(th[0], max(th[1], th[2])) - min(th[0], min(th[1], th[2]));
  }
  // Do not take the lowest value sensor because there might be the ball blocking it
  // Take the middle value instead
  sensor_th = rail_th[0] + 10;

  sensor_th = 50;

  Serial.print("Sensor threshold: ");
  Serial.println(sensor_th);
}

void loop() {
  curr_time = (long)millis();

  for (int i = 0; i < COILS; i++) {
    active_pos[i] = -1;
    activate[i] = false;
    for (int s = 0; s < 3; s++) {
      if (analogRead(sensors[i][s]) > sensor_th) {
        active_pos[i] = s;
        break;
      }
    }

    // The same coil fired again
    if (active_pos[i] != -1 && active_pos[i] == last_pos[i]) {
      active_pos[i] = -1;
      Serial.print(curr_time);
      Serial.print(": ");
      Serial.print("DOUBLE SKIP ON RAIL ");
      Serial.println(i);
      break;
    }

    // The ball was lost
    if (last_pos[i] != -1 && last_time[i] + TIME_OFF_MS < curr_time) {
      reset();
      Serial.print(curr_time);
      Serial.print(": ");
      Serial.print("LOST ON RAIL ");
      Serial.println(i);
      break;
    }

    if (active_pos[i] != -1 && curr_pos[i] == -1) {
      if (pallaggio_started_time == -1) {
        pallaggio_started_time = curr_time;
        Serial.println(pallaggio_started_time);
      }
      curr_pos[i] = active_pos[i];
      c[i][active_pos[i]].onNow(power[i]);

      Serial.print(curr_time);
      Serial.print(": ");
      Serial.print("On now ");
      Serial.print(i);
      Serial.print(" ");
      Serial.print(active_pos[i]);
      Serial.print(" power ");
      Serial.println(power[i]);
    } else if (active_pos[i] == -1 && curr_pos[i] != -1) {
      // we just left a coil
      if (last_pos[i] != -1) {
        if (curr_pos[i] != next(last_pos[i])) {
          Serial.print("SKIP FOR ");
          Serial.println(i);
          // ##### TODO ##### Do not activate on skip / backwards?
          period[i] = (curr_time - last_time[i]) / 2;
        } else {
          period[i] = curr_time - last_time[i];
        }
        activate[i] = true;
      }
      c[i][curr_pos[i]].off();
      Serial.print(curr_time);
      Serial.print(": ");
      Serial.print("Off ");
      Serial.print(i);
      Serial.print(" ");
      Serial.println(curr_pos[i]);

      mettiicazzodicommenti_pos[i] = curr_pos[i];

      last_pos[i] = curr_pos[i];
      last_time[i] = curr_time;
      curr_pos[i] = -1;
    }
  }

  // init phase
  for (int i = 0; i < COILS; i++) {
    power[i] = 1.0;
  }


  if (mettiicazzodicommenti_pos[0] == 0 && mettiicazzodicommenti_pos[1] == 0 && pallaggio_started_time != -1 && ((pallaggio_started_time + MINIMUM_PALLAGGIO_TIME) <= curr_time)) {
    Serial.println("Closing Chamber...");
    mettiicazzodicommenti_pos[0] = -1;
    mettiicazzodicommenti_pos[1] = -1;
    rs.closeChamber();
  }

  c[0][0].tick();
  c[0][1].tick();
  c[0][2].tick();
  c[1][0].tick();
  c[1][1].tick();
  c[1][2].tick();

  rs.update();
}
