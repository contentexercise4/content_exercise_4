
/*
2019.5.8
作成者：関根匠海
ver 1.0.0
*/

/*
[ver情報]
1.0.0
Charactorクラスを設計
Playerクラス(Charactorクラスを仮想継承)を設計
Enemyクラス(Chractorクラスを仮想継承)を設計
Itemクラス(Playerクラス, Enemyクラスを多重継承)を設計
*/

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <random>

using namespace std;
#define INFTY 2000000000        //無限大 = 20億

class Charactor{
private:
	string name;                //キャラクター名
	double PosX, PosZ;          //位置座標
	double SpeedX, SpeedZ;      //1Frameで移動する距離
	int LifePoint;              //ライフポイント
    int AttriVal;               //属性値(Attribute value) 0:Default 1:Player 2:Enemy(通常) 3:Enemy(攻撃力ゼロ) 4:Enemy(無力化)
	int size;                   //キャラサイズ
public:
    //コンストラクタ
    Charactor(){
        this->name = "SampleName";
        this->PosX = 0.0;
        this->PosZ = 10.0;
        this->SpeedX = 1.0;
        this->SpeedZ = 1.0;
        this->LifePoint = 100;
        this->AttriVal = 0;
        this->size = 10;
    }
    //コンストラクタ (属性値だけは必ず引数にする)
    Charactor(int AttriVal){
        this->name = "SampleName";
        this->PosX = 0.0;
        this->PosZ = 10.0;
        this->SpeedX = 1.0;
        this->SpeedZ = 1.0;
        this->LifePoint = 100;
        this->AttriVal = AttriVal;
        this->size = 10;
    }
    Charactor(string name, double PosX, double PosZ, double SpeedX, double SpeedZ, int LifePoint, int AttriVal, int size)
    {
        this->name = name;
        this->PosX = PosX;
        this->PosZ = PosZ;
        this->SpeedX = SpeedX;
        this->SpeedZ = SpeedZ;
        this->LifePoint = LifePoint;
        this->AttriVal = AttriVal;
        this->size = size;
    }
    //ライフを増減
    void ChangeLife(int LifeChange){
        LifePoint += LifeChange;
    }
    //ライフを返す
    int ReturnLife(){
        return LifePoint;
    }
    //ライフを無限(20億)にする. 練習モードの想定(バグの組み込み)
    void SetInftyLife(){
        this->LifePoint = INFTY;
    }
    //速度をセット
	void SetSpeed(int SpeedX, int SpeedZ) {
		this->SpeedX = SpeedX;
		this->SpeedZ = SpeedZ;
	}
	//X軸の速度を返す
	int ReturnSpeedX() {
		return SpeedX;
	}
	//Y軸の速度を返す
	int ReturnSpeedZ() {
		return SpeedZ;
	}
    //クラス変数の値（ステータス）の表示
    void PrindData(){
        printf("Attribute : ");
        if(AttriVal==0){
            printf("Default\n");
        }
        if(AttriVal==1){
            printf("Player\n");
        }
        if(AttriVal==2){
            printf("Enemy(Normal)\n");
        }
        if(AttriVal==3){
            printf("Enemy(AttackZero)\n");
        }
        if(AttriVal==4){
            printf("Enemy(Neutralization)");
        }
        if(AttriVal==5){
            printf("Item\n");
        }
        cout << "name : " << name << endl;
        printf("Position : (%2.2f, %2.2f, %2.2f)\n", PosX, 0, PosZ);
        printf("SpeedX : %2.2f SpeedZ : %2.2f\n", SpeedX, SpeedZ);
        printf("LifePoint : %3d\n", LifePoint);
        printf("size : %2d\n", size);
        putchar('\n');
    }
};

class Player : public virtual Charactor{
private:

public:
	//プレイヤーの向きを数字で管理
	int PlayerDirX, PlayerDirZ;             //現在のプレイヤーの向き
	int PastPlayerDirX, PastPlayerDirZ;     //直前のプレイヤーの向き

	//コンストラクタ
	Player() :Charactor(1) {
		PlayerDirX = 0;         //特になし
		PlayerDirZ = 1;         //画面奥向き
		PastPlayerDirX = 0;     //初期化
		PastPlayerDirZ = 1;     //初期化
	}
	Player(string name, double PosX, double PosZ, double SpeedX, double SpeedZ, int LifePoint, int AttriVal, int size)
        :Charactor(name, PosX, PosZ, SpeedX, SpeedZ, LifePoint, AttriVal, size) {
		PlayerDirX = 0;
		PlayerDirZ = 1;
		PastPlayerDirX = 0;
		PastPlayerDirZ = 1;
	}
	Player(string name, double PosX, double PosZ, double SpeedX, double SpeedZ, int LifePoint, int AttriVal, int size, int PlayerDirX, int PlayerDirZ)
        :Charactor(name, PosX, PosZ, SpeedX, SpeedZ, LifePoint, AttriVal, size) {
		PastPlayerDirX = this->PlayerDirX;
		PastPlayerDirZ = this->PlayerDirZ;
        this->PlayerDirX = PlayerDirX;
		this->PlayerDirZ = PlayerDirZ;
	}
};

class Enemy : public virtual Charactor{
private:

public:
	//敵の向きを数字で管理
	int EnemyDirX, EnemyDirZ;           //現在の敵の向き
	int PastEnemyDirX, PastEnemyDirZ;   //直前の敵の向き
	int AttackPower;                    //攻撃力. プレイヤーと衝突したときにプレイヤーの削られるライフに相当
	bool ExistFlag;                     //存在フラグ
    bool NeutrFlag;                     //プレイヤーが何らかの敵無力化アイテムを取得したときのためのフラグ. true状態でプレイヤーに衝突すると敵は消滅する
    bool AttackZeroFlag;                //敵がプレイヤーにダメージを与えたとき, 一定時間敵の攻撃力をゼロにするためのフラグ. これがないと当たり判定中にずっとプレイヤーはダメージを受ける
    int NeutrTime;                      //敵の攻撃力がゼロになる時間. Neutralization Time(無力化時間)

	//コンストラクタ
	Enemy() :Charactor(2) {
		EnemyDirX = 0;
		EnemyDirZ = 1;
		PastEnemyDirX = 0;
		PastEnemyDirZ = 1;
        AttackPower = 50;
		ExistFlag = false;
		NeutrFlag = false;
        AttackZeroFlag = false;
        NeutrTime = 3;
	}
	Enemy(string name, double PosX, double PosZ, double SpeedX, double SpeedZ, int LifePoint, int AttriVal, int size)
        :Charactor(name, PosX, PosZ, SpeedX, SpeedZ, LifePoint, AttriVal, size) {
		EnemyDirX = 0;
		EnemyDirZ = 1;
		PastEnemyDirX = 0;
		PastEnemyDirZ = 1;
        AttackPower = 50;
		ExistFlag = false;
		NeutrFlag = false;
        AttackZeroFlag = false;
        NeutrTime = 3;
	}
	Enemy(string name, double PosX, double PosZ, double SpeedX, double SpeedZ, int LifePoint, int AttriVal, int size, int EnemyDirX, int EnemyDirZ)
        :Charactor(name, PosX, PosZ, SpeedX, SpeedZ, LifePoint, AttriVal, size) {
		PastEnemyDirX = this->EnemyDirX;
		PastEnemyDirZ = this->EnemyDirZ;
        this->EnemyDirX = EnemyDirX;
		this->EnemyDirZ = EnemyDirZ;
		AttackPower = 50;
		ExistFlag = false;
		NeutrFlag = false;
        AttackZeroFlag = false;
        NeutrTime = 3;
	}
};

/*
多重継承・仮想継承
参考URL(https://programming-place.net/ppp/contents/cpp/language/030.html#multiple_inheritance)
*/

class Item : public Player, public Enemy{
private:
    int AttriVal = 5;     //Itemの属性値 = 5

public:
    //プレイヤーのライフを増減する
    void ChangePlayerLife(int ChangeLife){
        this->Player::ChangeLife(ChangeLife);
    }
    //プレイヤーのスピードを増減させる
    void ChangePlayerSpeed(int ChangeSpeed){
        int PrevSpeedX = this->Player::ReturnSpeedX();
        int PrevSpeedZ = this->Player::ReturnSpeedZ();
        this->Player::SetSpeed(PrevSpeedX+ChangeSpeed, PrevSpeedZ+ChangeSpeed);
    }
    //敵のスピードを増減させる
    void ChangeEnemySpeed(int ChangeSpeed){
        int PrevSpeedX = this->Enemy::ReturnSpeedX();
        int PrevSpeedZ = this->Enemy::ReturnSpeedZ();
        this->Enemy::SetSpeed(PrevSpeedX+ChangeSpeed, PrevSpeedZ+ChangeSpeed);
    }
};

int main(){
    Charactor* TMF = new Charactor();
    Player* Sagara = new Player();
    Enemy* Nakamura = new Enemy(); 
    Item* Takumi = new Item();

    TMF->PrindData();
    Sagara->PrindData();
    Nakamura->PrindData();
    Takumi->PrindData();

    delete TMF;
    delete Sagara;
    delete Nakamura;
    delete Takumi;
    
    return 0;
}