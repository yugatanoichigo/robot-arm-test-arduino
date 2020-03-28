int first_load = 1;     //初期読み込み（0 = 実行する, 1 = 実行しない）
int first_load_res = 0; //動作確認結果

int se01ec = 0, se01cn = 0;    //肩01（エンコーダ, 操作）
int se02ec = 0, se02cn = 0;    //肩02（エンコーダ, 操作）
int se03ec = 0, se03cn = 0;    //肘03（エンコーダ, 操作）

int moved01 = 0;    //移動回数（肩01）
int moved02 = 0;    //移動回数（肩02）
int moved03 = 0;    //移動回数（肘03）

int se01cn_tmp = 0;   //肩01（操作）仮保存
int se02cn_tmp = 0;   //肩02（操作）仮保存
int se03cn_tmp = 0;   //肘03（操作）仮保存

int way01 = 0, way01_cnt = 0, way01_cnt_tmp = 0;  //方向（肩01）
int way02 = 0, way02_cnt = 0, way02_cnt_tmp = 0;  //方向（肩02）
int way03 = 0, way03_cnt = 0, way03_cnt_tmp = 0;  //方向（肘03）

int error[3], error_b[3]; //エラー

int num = 0;

/* 入力ピン番号 */
int se01ec_pin = 0, se01cn_pin = 1;    //肩01（エンコーダ, 操作）
int se02ec_pin = 2, se02cn_pin = 3;    //肩02（エンコーダ, 操作）
int se03ec_pin = 4, se03cn_pin = 5;    //肘03（エンコーダ, 操作）

/* 出力ピン番号 */
int sh01a = 9 , sh01b = 10, sh01p = 11;  //肩01
int sh02a = 4 , sh02b = 5 , sh02p = 6;   //肩02
int sh03a = 12, sh03b = 8 , sh03p = 3;   //肘03
int error_out = 2;

void setup() {
  pinMode(sh01a, OUTPUT); //肩01 A
  pinMode(sh01b, OUTPUT); //肩01 B

  pinMode(sh02a, OUTPUT); //肩02 A
  pinMode(sh02b, OUTPUT); //肩02 B

  pinMode(sh03a, OUTPUT); //肘03 A
  pinMode(sh03b, OUTPUT); //肘03 B

  pinMode(error_out, OUTPUT); //エラーブザー
  Serial.begin(9600);

  Serial.println("\n## load ##");
  delay(1000);
}

void loop() {
  /* ## 初期読み込み（動作チェック） ## */
  if (first_load == 0) {
    first_load = 1;
    firstload(); //初期読み込み関数
  }


  if (first_load_res == 0) {
    /* ## 肩01の動作 ## */
    if (moved01 < 30 || moved02 < 30 || moved03 < 30) {
      if (error_b[0] == 0 || error_b[1] == 0) {
        if (se01cn < se01ec - 10 || se02cn < se02ec - 10 || se03cn < se03ec - 10) {  //上の動作
          if (se01cn < se01ec - 10) {
            brake(sh01a, sh01b, sh01p);    //ブレーキ
            back(sh01a, sh01b, sh01p, 255);//上の動作

            delay(50); //下の動作時間
          }

          stop(sh01a, sh01b, sh01p); //モータ（肩01）の一時停止
          
          if (se02cn < se02ec - 10) {
            brake(sh02a, sh02b, sh02p);    //ブレーキ
            go(sh02a, sh02b, sh02p, 255);  //上の動作

            delay(20);
          }
          
          stop(sh02a, sh02b, sh02p); //モータ（肩01）の一時停止
          
          if (se03cn < se03ec - 10) {
            brake(sh03a, sh03b, sh03p);    //ブレーキ
            go(sh03a, sh03b, sh03p, 255);  //上の動作

            delay(10);
          }

          stop(sh03a, sh03b, sh03p); //モータ（肩01）の一時停止

          if (se01cn < se01ec - 10) moved01++;  //移動回数の加算
          if (se02cn < se02ec - 10) moved02++;  //移動回数の加算
          if (se03cn < se03ec - 10) moved03++;  //移動回数の加算

          if (se01cn < se01ec - 10) {
            if (way01 == 1) { //方向の変更
              way01 = 0;      //方向変数の変更
              way01_cnt++;    //方向変更回数の加算
            }
          }

          if (se02cn < se02ec - 10) {
            if (way02 == 1) { //方向の変更
              way02 = 0;      //方向変数の変更
              way02_cnt++;    //方向変更回数の加算
            }
          }

          if (se03cn < se03ec - 10) {
            if (way03 == 1) { //方向の変更
              way03 = 0;      //方向変数の変更
              way03_cnt++;    //方向変更回数の加算
            }
          }

        } else if (se01cn > se01ec + 10 || se02cn > se02ec + 10 || se03cn > se03ec + 10) { //下の動作
          if (se01cn > se01ec + 10) {
            brake(sh01a, sh01b, sh01p);      //ブレーキ
            go(sh01a, sh01b, sh01p, 255);    //下の動作

            delay(5); //下の動作時間
          }
          
          stop(sh01a, sh01b, sh01p); //モータ（肩01）の一時停止
          
          if (se02cn > se02ec + 10) {
            brake(sh02a, sh02b, sh02p);      //ブレーキ
            back(sh02a, sh02b, sh02p, 255);  //下の動作

            delay(20); //下の動作時間
          }
          
          stop(sh02a, sh02b, sh02p); //モータ（肩01）の一時停止
          
          if (se03cn > se03ec + 10) {
            brake(sh03a, sh03b, sh03p);      //ブレーキ
            back(sh03a, sh03b, sh03p, 255);  //下の動作

            delay(10); //下の動作時間
          }

          stop(sh03a, sh03b, sh03p); //モータ（肩01）の一時停止

          if (se01cn > se01ec + 10) moved01++;  //移動回数の加算
          if (se02cn > se02ec + 10) moved02++;  //移動回数の加算
          if (se03cn > se03ec + 10) moved03++;  //移動回数の加算

          if (se01cn > se01ec + 10) {
            if (way01 == 0) { //方向の変更
              way01 = 1;      //方向変数の変更
              way01_cnt++;    //方向変更回数の加算
            }
          }

          if (se02cn > se02ec + 10) {
            if (way02 == 0) { //方向の変更
              way02 = 1;      //方向変数の変更
              way02_cnt++;    //方向変更回数の加算
            }
          }

          if (se03cn > se03ec + 10) {
            if (way03 == 0) { //方向の変更
              way03 = 1;      //方向変数の変更
              way03_cnt++;    //方向変更回数の加算
            }
          }
        }

        if (se01cn < se01ec - 10) {  //上の動作
          moved01 = moved01;
        } else if (se01cn > se01ec + 10) { //下の動作
          moved01 = moved01;
        } else {
          moved01 = 0;
        }

        if (se02cn < se02ec - 10) {  //上の動作
          moved02 = moved02;
        } else if (se02cn > se02ec + 10) { //下の動作
          moved02 = moved02;
        } else {
          moved02 = 0;
        }

        if (se03cn < se03ec - 10) {  //上の動作
          moved03 = moved03;
        } else if (se03cn > se03ec + 10) { //下の動作
          moved03 = moved03;
        } else {
          moved03 = 0;
        }
        
      }
    }

    /*
    //（肩01）動作が0回ではなく、動作が30回未満の場合
    if (moved01 != 0 && moved01 < 30) {
      num = 0;

      if (error[num] == 0 && error_b[num] == 0) {
        //Serial.print("moved01 : ");
        //Serial.println(moved01);
      }
    }

    //（肩02）動作が0回ではなく、動作が30回未満の場合
    if (moved02 != 0 && moved02 < 30) {
      num = 1;

      if (error[num] == 0 && error_b[num] == 0) {
        //Serial.print("moved02 : ");
        //Serial.println(moved02);
      }
    }

    //（肘03）動作が0回ではなく、動作が30回未満の場合
    if (moved03 != 0 && moved03 < 30) {
      num = 2;

      if (error[num] == 0 && error_b[num] == 0) {
        //Serial.print("moved03 : ");
        //Serial.println(moved03);
      }
    }
    */


    /* ## エラー判定 ## */
    //（肩01）動作が30回以上の場合、エラー（モータが動いていない）
    if (moved01 >= 30) {
      num = 0;

      if (error[num] == 0) {
        error[num] = 1;
        error_func(5);

        //Serial.println("error::cannot moved01.");
      }
    }
    //（肩01）10回以上、上下を行き来した場合、エラー（位置が定まらない）
    if (way01_cnt >= 10) {
      num = 0;

      if (error_b[num] == 0) {
        //Serial.println("stop move01.");

        error_b[num] = 1;
        error_func(3);
      }
    }

    //（肩02）動作が30回以上の場合、エラー（モータが動いていない）
    if (moved02 >= 30) {
      num = 0;

      if (error[num] == 0) {
        error[num] = 1;
        error_func(5);

        //Serial.println("error::cannot moved02.");
      }
    }
    //（肩02）10回以上、上下を行き来した場合、エラー（位置が定まらない）
    if (way02_cnt >= 10) {
      num = 1;

      if (error_b[num] == 0) {
        //Serial.println("stop move02.");

        error_b[num] = 1;
        error_func(3);
      }
    }

    //（肘03）動作が30回以上の場合、エラー（モータが動いていない）
    if (moved03 >= 30) {
      num = 2;

      if (error[num] == 0) {
        error[num] = 1;
        error_func(5);

        //Serial.println("error::cannot moved02.");
      }
    }
    //（肘03）10回以上、上下を行き来した場合、エラー（位置が定まらない）
    if (way03_cnt >= 10) {
      num = 2;

      if (error_b[num] == 0) {
        //Serial.println("stop move02.");

        error_b[num] = 1;
        error_func(3);
      }
    }


    /* ## モータ停止処理 ## */
    stop(sh01a, sh01b, sh01p); //モータ（肩01）の一時停止
    stop(sh02a, sh02b, sh02p); //モータ（肩02）の一時停止
    stop(sh03a, sh03b, sh03p); //モータ（肘03）の一時停止


    /* ## センサ値の読み込み ## */
    //センサ値の読み込み
    se01ec = analogRead(se01ec_pin);   //肩01（エンコーダ）
    se01cn = analogRead(se01cn_pin);   //肩01（操作）
    se02ec = analogRead(se02ec_pin);   //肩02（エンコーダ）
    se02cn = analogRead(se02cn_pin);   //肩02（操作）
    se03ec = analogRead(se03ec_pin);   //肘03（エンコーダ）
    se03cn = analogRead(se03cn_pin);   //肘03（操作）

    //センサ値の調節
    se01cn = map(se01cn, 0, 1023, 30, 500) * 2;  //肩01（操作）
    se02cn = map(se02cn, 0, 1023, 70, 300) * 3;  //肩02（操作）
    se03cn = map(se03cn, 0, 1023, 70, 300) * 4;  //肘03（操作）

    se01ec = se01ec / 20 * 10;         //肩01（エンコーダ）
    se01cn = (se01cn / 20 * 10) - 30;  //肩01（操作）
    se02ec = (se02ec / 20 * 10) + 10;  //肩02（エンコーダ）
    se02cn = (se02cn / 20 * 10) + 50;  //肩02（操作）
    se03ec = se03ec / 20 * 10;         //肘03（エンコーダ）
    se03cn = (se03cn / 20 * 10) - 150;  //肘03（操作）

    se03cn = 500 - se03cn;

    /* ## 出力 ## */
    /*
    Serial.print("se03ec: ");
    Serial.print(se03ec);
    Serial.print("\tse03ct: ");
    Serial.println(se03cn);
    */
    //110 - 500

    delay(5);  //一時停止時間

    //（肩01）操作センサ値が変わった場合、各変数のリセット
    if (se01cn != se01cn_tmp) {
      moved01 = 0;      //移動回数

      num = 0;
      error[num] = 0;      //エラーリセット（モータが動いていない）
      error_b[num] = 0;    //エラーリセット（位置が定まらない）

      way01_cnt = 0;  //行き来回数

      //Serial.println("se01cn change");
    }

    //（肩02）操作センサ値が変わった場合、各変数のリセット
    if (se02cn != se02cn_tmp) {
      moved02 = 0;      //移動回数

      num = 1;
      error[num] = 0;      //エラーリセット（モータが動いていない）
      error_b[num] = 0;    //エラーリセット（位置が定まらない）

      way02_cnt = 0;  //行き来回数

      //Serial.println("se02cn change");
    }

    //（肘03）操作センサ値が変わった場合、各変数のリセット
    if (se03cn != se03cn_tmp) {
      moved03 = 0;      //移動回数

      num = 2;
      error[num] = 0;      //エラーリセット（モータが動いていない）
      error_b[num] = 0;    //エラーリセット（位置が定まらない）

      way03_cnt = 0;  //行き来回数

      //Serial.println("se03cn change");
    }


    //（肩01）行き来した場合、その回数を出力
    if (way01_cnt != way01_cnt_tmp) {
      //Serial.print("way01_cnt : ");
      //Serial.println(way01_cnt);
    }

    //（肩02）行き来した場合、その回数を出力
    if (way02_cnt != way02_cnt_tmp) {
      //Serial.print("way02_cnt : ");
      //Serial.println(way02_cnt);
    }

    //（肘03）行き来した場合、その回数を出力
    if (way03_cnt != way03_cnt_tmp) {
      //Serial.print("way03_cnt : ");
      //Serial.println(way03_cnt);
    }


    /* ## 前回変数値の記憶 ## */
    se01cn_tmp = se01cn;
    way01_cnt_tmp = way01_cnt;

    se02cn_tmp = se02cn;
    way02_cnt_tmp = way02_cnt;

    se03cn_tmp = se03cn;
    way03_cnt_tmp = way02_cnt;
  }
}


/* #######################################################################
                                     FUNCTION
   #######################################################################*/

/* ######### 初期読み込み ######## */
void firstload() {
  Serial.println("START first load.");

  stop(sh01a, sh01b, sh01p);
  stop(sh02a, sh02b, sh02p);
  delay(500);

  int val_f = 0;
  int val_f_count = 0;

  while (val_f < 210) {   //肩01（下）
    val_f = analogRead(se01ec_pin);
    val_f_count++;
    go(sh01a, sh01b, sh01p, 200);

    Serial.print("sh 01 DOWN: ");
    Serial.print(val_f_count);
    Serial.print("\tec val: ");
    Serial.println(val_f);

    if (val_f_count > 400) break;
  }

  val_f_count = 0;
  stop(sh01a, sh01b, sh01p);
  stop(sh02a, sh02b, sh02p);
  delay(500);

  while (val_f > 40) {   //肩01（上）
    val_f = analogRead(se01ec_pin);
    val_f_count++;
    back(sh01a, sh01b, sh01p, 200);

    Serial.print("sh 01 UP: ");
    Serial.print(val_f_count);
    Serial.print("\tec val: ");
    Serial.println(val_f);

    if (val_f_count > 400) break;
  }

  val_f_count = 0;
  stop(sh01a, sh01b, sh01p);
  stop(sh02a, sh02b, sh02p);
  delay(500);

  while (val_f < 210) {   //肩01（下）
    val_f = analogRead(se01ec_pin);
    val_f_count++;
    go(sh01a, sh01b, sh01p, 200);

    Serial.print("sh 01 DOWN: ");
    Serial.print(val_f_count);
    Serial.print("\tec val: ");
    Serial.println(val_f);

    if (val_f_count > 400) break;
  }

  val_f_count = 0;
  stop(sh01a, sh01b, sh01p);
  stop(sh02a, sh02b, sh02p);
  delay(500);

  while (val_f < 700) {   //肩02（右）
    val_f = analogRead(se02ec_pin);
    val_f_count++;
    back(sh02a, sh02b, sh02p, 255);

    Serial.print("sh 02 RIGHT: ");
    Serial.print(val_f_count);
    Serial.print("\tec val: ");
    Serial.println(val_f);

    if (val_f_count > 400) break;
  }

  val_f_count = 0;
  stop(sh01a, sh01b, sh01p);
  stop(sh02a, sh02b, sh02p);
  delay(500);

  while (val_f > 460) {   //肩02（左）
    val_f = analogRead(se02ec_pin);
    val_f_count++;
    go(sh02a, sh02b, sh02p, 255);

    Serial.print("sh 02 LEFT: ");
    Serial.print(val_f_count);
    Serial.print("\tec val: ");
    Serial.println(val_f);

    if (val_f_count > 400) break;
  }

  val_f_count = 0;
  stop(sh01a, sh01b, sh01p);
  stop(sh02a, sh02b, sh02p);
  delay(500);

  delay(500);

  //動作確認後の位置確認
  int flg01 = 0, flg02 = 0;

  se01ec = analogRead(se01ec_pin);
  se02ec = analogRead(se02ec_pin);

  if (se01ec > 150) flg01 = 1;   //上
  if (se02ec > 300 || se02ec < 450) flg02 = 1;  //端と端の間

  if (flg01 && flg02) {
    Serial.println("FINISHED first load.");
    first_load_res = 0;
  } else {
    Serial.println("## DONT FINISHED first load. ##");
    first_load_res = 1;
  }
}

/* ######### 肩01（下） ######## */
//引数：入力A, 入力B, 入力PWM, PWM値
void go(int a, int b, int p, int pwm) {
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  analogWrite(p , pwm);
}

/* ######### 肩01（上） ######## */
//引数：入力A, 入力B, 入力PWM, PWM値
void back(int a, int b, int p, int pwm) {
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  analogWrite(p , pwm);
}

/* ######### 肩01（停止） ######## */
//引数：入力A, 入力B, 入力PWM
void stop(int a, int b, int p) {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  analogWrite(p , 0);
}

/* ######### 肩01（ブレーキ） ######## */
//引数：入力A, 入力B, 入力PWM
void brake(int a, int b, int p) {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  analogWrite(p , 255);
}

/* ######### エラーブザー ######## */
//引数：ブザーを鳴らす回数
void error_func(int cnt) {
  int i;

  for (i = 0; i < cnt; i++) {
    digitalWrite(error_out, HIGH);
    delay(40);
    digitalWrite(error_out, LOW);
    delay(40);
  }
}
