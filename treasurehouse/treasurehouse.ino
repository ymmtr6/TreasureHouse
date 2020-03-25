#define tap_delay 300
#define skill1 3
#define skill2 2
#define skill3 4
#define attack 6
#define select 5
#define special 7

#define AP_MAX 140
#define AP_COST 40
int AP = 84;
int COUNT = 20;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(2, OUTPUT); // 2nd skill (2)
  pinMode(3, OUTPUT); // Select Special
  pinMode(4, OUTPUT); // 3rd skill (1)
  pinMode(5, OUTPUT); // 1st skill (3)
  pinMode(6, OUTPUT); // Attack
  pinMode(7, OUTPUT); // select servernt
  pinMode(8, OUTPUT); // none

  Serial.println("FG0 Auto TreasureHouse Orbit System var1.0");
  Serial.println("");
  Serial.print("AP_MAX: ");
  Serial.println(AP_MAX);
  Serial.print("AP_COST: ");
  Serial.println(AP_COST);
  Serial.print("AP: ");
  Serial.println(AP);
  Serial.print("COUNT: ");
  Serial.println(COUNT);
  Serial.print("Apple: ");
  Serial.println( (AP_COST * COUNT - AP) / AP_MAX );
  Serial.println("USAGE: s:start, a:prepare, b:battle, c:after, 2-14:tap");
  Serial.println("");
}

void loop() {
  // put your main code here, to run repeatedly:
  int inputchar;
  inputchar = Serial.read();

  if(inputchar - 0x30 > 1 && inputchar - 0x30 < 14){
    // 物理pin番号指定でtap
    Serial.println(inputchar - 0x30);
    tap(inputchar - 0x30);
    delay(1000);
  }else if(inputchar == 'a'){
    // 前処理
    ap(-AP_COST);
    Battle_prepare();
  }else if(inputchar == 'b'){
    // battle処理
    Battle();
  }else if(inputchar == 'c'){
    // 終了処理
    Battle_after();
  }else if(inputchar == 'p'){
    // ap追加
    ap(AP_MAX);
  }else if(inputchar == 'm'){
    // ap消費
    ap(-AP_COST);
  } else if(inputchar == 's'){
    // Auto処理
    for(int i = 0; i < COUNT; i++){
      Serial.print("[info] Count: ");
      Serial.println(i+1);
      all();
    }
    Serial.println("[info] Finish!!");
  }
}

void all(){
  // 全てを実行
  ap(-40); // ap消費
  delay(3000);
  Battle_prepare(); // 前処理(スキル選択)
  Battle(); // turn 1
  delay(10000);
  Battle(); // turn 2
  delay(10000);
  Battle(); // turn 3
  delay(10000);
  Battle_after(); // 結果表示・リンゴ消費・フレンド選択
  delay(10000);
}

void ap(int num){
  // APを追加する．
    AP += num;
    Serial.print("[info] AP:");
    Serial.println(AP);
}

void Battle_prepare(){
  Serial.println("[debug] start skill select");
  
  tap(skill1);
  delay(3400);
  tap(skill2);
  tap(select);
  delay(2400);
  tap(skill3);
  tap(select);
  delay(2400);

  Serial.println("[debug] finish skill select");
}

void Battle(){
  Serial.println("[debug] start battle turn");
  tap(attack);
  delay(3000);
  tap(special);
  //tap(skill2);
  tap(skill3);
  tap(attack);

  // 演出終了まで待機
  delay(20000);
  
  Serial.println("[debug] finish battle turn");
}

void Battle_after(){
  Serial.println("[debug] start battle after select");

  // 絆レベル
  tap(attack);
  delay(1000);
  // マスターレベル
  tap(attack);
  delay(1000);
  // 戦利品
  tap(attack);
  delay(1000);
  // 連続してクエストを行う -> はい
  tap(skill3);
  delay(1000);

  // もしAPが足りない場合は...
  if (AP < 40){
  // リンゴを選択
    tap(select);
    delay(1000);
  // はい
    tap(skill3);
    ap(AP_MAX);
  }
  delay(3000);
  // フレンドを選択
  tap(skill2);

  Serial.println("[debug] finish battle after select");
}

int tap(int pinNum){
  digitalWrite(pinNum, HIGH);
  delay(tap_delay);
  digitalWrite(pinNum, LOW);
  delay(tap_delay);
}
