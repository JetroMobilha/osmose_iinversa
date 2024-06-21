// Codigo que funciona com display associado

// Inicialização do LDC
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 10, d3 =9, d2 = 8, d1 = 7;
LiquidCrystal lcd(rs, en, d4, d3, d2, d1);
// Inicialização LDC

#define boiaNivelAlto                  27
#define boiaNivelBaixoBruta            28
#define boiaNivelBaixoTanqueFloushing  29
#define boiaNivelAltoTanqueFloushing   30
#define pressostatoNivelBaixo          31
#define pressostatoNivelAlto           32
#define avariaBombaAltaPressao         33
#define avariaBombaDoseadoraRO         34
#define lavagemFiltro                  35
#define releSequenciaFase              36
#define funcionamentobombaAltaPressao  37
#define rearmeAlarme                   38              
//Saída
#define led                            22
#define valvulaEntrada                 23
#define valvulaFloushing               24
#define ordemFuncioBombaAltaPressao    25
#define ordemFuncioBombaDoseadora      26


bool mBoiaNivelAlto=0;
bool leitura2=0;
bool machine=false;
bool machineRun=false;
bool mudarAlarme=true;
bool mPressostatoNivelAlto=0;
bool mBoiaNivelBaixoBruta=0;
bool mPressostatoNivelBaixo=0;
bool mBoiaNivelBaixoTanqueFlousing=0;
bool mBoiaNivelAltoTanqueFlousing =0;
bool mRearmeAlarme=0;
bool mAvariaBombaDoseadora=0;
bool mAvariaBombaAltaPressao=0;
bool mReleSequenciaFase=1;
bool mLavagemFiltro=0;
bool mLavagemFiltroTermino=0;
bool mEmFlouching=0;
bool mFuncionamentobombaAltaPressao=0;
int cont1=0;
String textoDisplay="Inicializando";
String textoDisplayDescricao=" ";
 
// Variaveis usadas para reconhecer quais dos alermes ja foi edentificado 
int alarme=0;
bool tipoAlarme1=false;
bool tipoAlarme2=false;
bool tipoAlarme3=false;
bool tipoAlarme4=false;
bool tipoAlarme5=false;


//---Propótipos das Funções Auxiliares---
void contador();
void leituraEntradas();
void ligarSaidas();
void desligarSaidas();
void verificarPressaoB();
void ligarLedAlarme();
bool verifcandoAlarme();
void definirAlarme();
void desligarBombaAltaPressao();
void ligarBombaAltaPressao();
void tanqueCheio();
void encherTanqueFloushing();
bool isRun();
void reSetAlarme();
void filtroLava();
void sistemaEmBaixaPresao();
void mensagemDisplay(String titulo,String descrisao);



void setup() 
{
  lcd.begin(16, 2);
  lcd.print(textoDisplay);
  
  Serial.begin(9600);
  
  //entradas
  pinMode(boiaNivelAlto,                INPUT);              
  pinMode(boiaNivelBaixoBruta,          INPUT);            
  pinMode(boiaNivelBaixoTanqueFloushing,INPUT);  
  pinMode(boiaNivelAltoTanqueFloushing, INPUT);
  pinMode(pressostatoNivelBaixo,        INPUT);          
  pinMode(pressostatoNivelAlto,         INPUT);           
  pinMode(avariaBombaAltaPressao,       INPUT);        
  pinMode(avariaBombaDoseadoraRO,       INPUT);         
  pinMode(lavagemFiltro,                INPUT);                 
  pinMode(releSequenciaFase,            INPUT);              
  pinMode(funcionamentobombaAltaPressao,INPUT);  
  pinMode(rearmeAlarme,                 INPUT);
  //saidas
  pinMode(valvulaEntrada,              OUTPUT);
  pinMode(valvulaFloushing,            OUTPUT);   
  pinMode(ordemFuncioBombaAltaPressao, OUTPUT); 
  pinMode(ordemFuncioBombaDoseadora,   OUTPUT);  
  pinMode(led, OUTPUT); 

  digitalWrite(valvulaEntrada,HIGH);
  digitalWrite(valvulaFloushing,HIGH);   
  digitalWrite(ordemFuncioBombaAltaPressao, HIGH); 
  digitalWrite(ordemFuncioBombaDoseadora,  HIGH);  
  digitalWrite(led,  HIGH);  
}


void loop() 
{
  leituraEntradas();
  ligarLedAlarme();
  definirAlarme();
   
  // Eliminar alerme se existir se o botão realme for precionado
  if(mRearmeAlarme == HIGH) {
    reSetAlarme();
  }
  
  if(isRun()){
    machine = true;
  }
 
  filtroLava();

  Serial.print("machine = ");
  Serial.println(machine);
  Serial.print("machineRun = ");
  Serial.println(machineRun);
  if(machine && !machineRun && mEmFlouching==0)
  { 
    ligarSaidas();
    mensagemDisplay("Valvula ","Entrada Aberta");
    verificarPressaoB();    
  }
  else if(verifcandoAlarme()) desligarMaquina();
   
  tanqueCheio();
  encherTanqueFloushing();
  
  if (machineRun==true && mPressostatoNivelBaixo == 0 && mEmFlouching ==0 )
  {
    alarme=1;
    tipoAlarme5=true;
    desligarMaquina();
  }

  sistemaEmBaixaPresao();

  delay(1000);
   
}//END loop


//--- Funções Auxiliares---
void encherTanqueFloushing()
{
  if(machineRun==true && mBoiaNivelAltoTanqueFlousing ==1 )
  {
    Serial.println("encher TanqueFloushing");
    digitalWrite(valvulaFloushing ,LOW);
    mensagemDisplay("Osmose","Encher Floushi");
  }
  else
  {   
    digitalWrite(valvulaFloushing ,HIGH);
    mensagemDisplay("Osmose","Em Producao");
  }
}

void tanqueCheio()
{
  if(mBoiaNivelAlto == 0 && mBoiaNivelBaixoTanqueFlousing ==1 && machineRun==true )
  {
    mensagemDisplay("Osmose","Em Floushing");
    Serial.println("Iniciar Floushing");
    mEmFlouching=1;
    desligarSaidas();  
  }
  else if(mBoiaNivelAlto == 0 && mBoiaNivelBaixoTanqueFlousing ==0 && machineRun==true )
  { 
    mEmFlouching=0;
    desligarMaquina();
    mensagemDisplay("Osmose","Tanque Cheio");
  } 
}

void sistemaEmBaixaPresao()
{
  if(tipoAlarme5 == true && mBoiaNivelBaixoTanqueFlousing ==1 && machineRun == 0)
  {
    Serial.println("Iniciar Floushing por baixa pressão");
    ligarBombaAltaPressao();  
    mEmFlouching=1;
    mensagemDisplay("Osmose","Em Floushing");
  }
  else if(tipoAlarme5 == true &&  machineRun == 0){ 
    mEmFlouching=0;
    Serial.println("Fim Floushing por baixa pressão");
    mensagemDisplay("Osmose Parado","Baixa Pressao");
    desligarBombaAltaPressao();
    desligarSaidas();
    alarme=0;
    tipoAlarme5=0;
    delay(30000);
  } 
}

void verificarPressaoB()
{  
  Serial.println("Esperando pressao do sistema ...");
  delay(4000);
  
  if(mPressostatoNivelBaixo == 1 && cont1 <3)
  {
    ligarBombaAltaPressao();
    machineRun=true;
    mensagemDisplay("Osmose","Em Producao");
  }
  else
  {
    contador();
  } 
}

void ligarLedAlarme()
{  
  switch(alarme)
  {
    case 1:
      if(mudarAlarme)
      {
        digitalWrite(led,LOW);
        mudarAlarme=false;
      }
      else
      {
        digitalWrite(led,HIGH);
        mudarAlarme= true;
      }
      break;
    case 2:
      digitalWrite(led, LOW);
      break;
    default:
      digitalWrite(led,HIGH);
  }
} 

void reSetAlarme()
{
   Serial.println("Rearme pressionado");

  // verificando se o alarme aparece pela primeiravez
  if(mPressostatoNivelAlto==1 && tipoAlarme1==true) 
  {
    alarme=0;
    machine=true;
    tipoAlarme1=false;
  } // verificando de o alareme ja foi reconhecido pela ultimavez de verificanção
  else if(mPressostatoNivelAlto==0 && tipoAlarme1==true)
  {
    alarme=2;
  }

  if(mAvariaBombaAltaPressao==0 && tipoAlarme2==true) 
  {
    alarme=0;
    machine=true;
    tipoAlarme2=false;
  }
  else if(mAvariaBombaAltaPressao==1 && tipoAlarme2==true)
  {
    alarme=2;
  }

  if( mAvariaBombaDoseadora==1 && tipoAlarme3==true) 
  {
    alarme=0;
    machine=true;
    tipoAlarme3=false;
  }
  else if(mAvariaBombaDoseadora==0 && tipoAlarme3==true)
  {
    alarme=2;
  }
   
  if( mReleSequenciaFase==1 && tipoAlarme4==true) 
  {
    alarme=0;
    machine=true;
    tipoAlarme4=false;
  }
  else if(mReleSequenciaFase==0 && tipoAlarme4==true)
  {
    alarme=2;
  }

  if(tipoAlarme5==true) 
  {
    alarme=0;
    tipoAlarme5=0;
  }
}

bool verifcandoAlarme()
{
  bool volta;
  if( alarme==1 || alarme==2) volta = true;
  else volta = false;

  return volta;      
}
 

void definirAlarme()
{
  // verificando se o alarme aparece pela primeiravez
  if(mPressostatoNivelAlto==0){
    if(mPressostatoNivelAlto==0 && tipoAlarme1==false){
      alarme=1;
      machine=false;
      machineRun=false;
      tipoAlarme1=true;
    } 

    mensagemDisplay("Alarme","Pressotato Alt");
    Serial.println("Alarme Pressostato Nivel Alto");
  } 

  if(mAvariaBombaAltaPressao==1){
    if(mAvariaBombaAltaPressao==1 && tipoAlarme2==false){
      alarme=1;
      machine=false;
      machineRun=false;
      tipoAlarme2=true;
    } 

    mensagemDisplay("Alarme","Bomba Alt");
    Serial.println("Alarme Avaria Bomba Alta Pressao");
  } 

  if( mAvariaBombaDoseadora==0){
    if( mAvariaBombaDoseadora==0 && tipoAlarme3==false){
      alarme=1;
      machine=false;
      machineRun=false;
      tipoAlarme3=true;  
    }  

    mensagemDisplay("Alarme","Doseadora");
    Serial.println("Alarme Avaria Bomba Doseadora");
  } 

  if( mReleSequenciaFase==0){
    if( mReleSequenciaFase==0 && tipoAlarme4==false){
      alarme=1;
      machine=false;
      machineRun=false;
      tipoAlarme4=true;
    } 

    mensagemDisplay("Alarme","Sequencia Fase");
    Serial.println("Alarme Rele Sequencia Fase");
  } 

  if(mPressostatoNivelBaixo==0 && tipoAlarme5==true){
    mensagemDisplay("Alarme","BAIXA PRESSAO");
    Serial.println("ALARME BAIXA PRESSAO");
  } 
}

bool isRun()
{
  bool volta =false;
  if( 
      mBoiaNivelAlto==1 && 
      mLavagemFiltro==0 &&  
      !verifcandoAlarme()
    )  volta = true;

  if(mLavagemFiltroTermino==1){
    mLavagemFiltroTermino=0;
    delay(30000);
  }
  return volta;    
}


void leituraEntradas()
{
  mBoiaNivelAlto = digitalRead(boiaNivelAlto);                          
  mBoiaNivelBaixoBruta = digitalRead(boiaNivelBaixoBruta);                     
  mBoiaNivelBaixoTanqueFlousing = digitalRead(boiaNivelBaixoTanqueFloushing);  
  mBoiaNivelAltoTanqueFlousing =digitalRead(boiaNivelAltoTanqueFloushing);
  mPressostatoNivelBaixo =digitalRead(pressostatoNivelBaixo);          
  mPressostatoNivelAlto =digitalRead(pressostatoNivelAlto);           
  mAvariaBombaAltaPressao =digitalRead(avariaBombaAltaPressao);        
  mAvariaBombaDoseadora =digitalRead(avariaBombaDoseadoraRO);         
  mLavagemFiltro=digitalRead(lavagemFiltro);                 
  mReleSequenciaFase =digitalRead(releSequenciaFase);              
  mFuncionamentobombaAltaPressao =digitalRead(funcionamentobombaAltaPressao);  
  mRearmeAlarme =digitalRead(rearmeAlarme); 
}//endleituraEntradas

void ligarSaidas()
{
   digitalWrite(valvulaEntrada, LOW);
   digitalWrite(ordemFuncioBombaDoseadora, LOW);
}//end ligarSaidas

void desligarSaidas()
{  
  digitalWrite(valvulaEntrada, HIGH);
  digitalWrite(ordemFuncioBombaDoseadora, HIGH);  
}

void desligarBombaAltaPressao()
{
  digitalWrite(ordemFuncioBombaAltaPressao,HIGH);
}

void desligarMaquina()
{
  desligarSaidas();
  if (mEmFlouching==0) desligarBombaAltaPressao();
  machineRun=false;
  machine=false;
  Serial.println("Maquina desligada");
}

void ligarBombaAltaPressao()
{
  Serial.print("Sistema em Producao: ");
  Serial.println(ordemFuncioBombaAltaPressao);
  digitalWrite(ordemFuncioBombaAltaPressao,LOW);
} 

// para verificar pressão de entrada da osmose
void contador(){
  cont1++;
  Serial.print("Sistema sem pressao :");
  Serial.println(cont1);

  if(cont1>=3)
  {
    cont1=0;
    machine=false;
    alarme=1;
    tipoAlarme5=true;
    Serial.println("Alarme de Baixa pressao");
  }
}

void filtroLava(){
  if(mLavagemFiltro==1)
  {
    mensagemDisplay("Filtro","Em Lavagem");
    Serial.println("Filtro em lavagem");
    desligarMaquina();
    mLavagemFiltroTermino=1;
  }
}

void mensagemDisplay(String titulo="",String descrisao=""){

  textoDisplay= titulo;
  textoDisplayDescricao= descrisao;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(titulo);

  lcd.setCursor(0, 1);
  lcd.print(descrisao);
}