#define boiaNivelAlto                  22
#define boiaNivelBaixoBruta            23
#define boiaNivelBaixoTanqueFloushing  24
#define boiaNivelAltoTanqueFloushing   25
#define pressostatoNivelBaixo          26
#define pressostatoNivelAlto           27
#define avariaBombaAltaPressao         28
#define avariaBombaDoseadoraRO         29
#define lavagemFiltro                  30
#define releSequenciaFase              31
#define funcionamentobombaAltaPressao  32
#define rearmeAlarme                   33              
//Saída
#define led                            8
#define valvulaEntrada                 7
#define valvulaFloushing               6
#define ordemFuncioBombaAltaPressao    5
#define ordemFuncioBombaDoseadora      4


bool mBoiaNivelAlto=0;
bool leitura2=0;
bool machine=false;
bool machineRun=false;
bool mudarAlarme=true;
bool mPressostatoNivelAlto=0;
bool mBoiaNivelBaixoBruta=1;
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



void setup() 
{
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
  Serial.print("Verificando = ");
  Serial.println(verifcandoAlarme());
  if(machine && !machineRun && mEmFlouching==0)
  {
    Serial.println("opcao3");
    ligarSaidas();
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
    Serial.println("Alarme de Paixa pressão");
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
  }
  else
  {   
    //Serial.println("tanque cheio");
    digitalWrite(valvulaFloushing ,HIGH);
  }
}

void tanqueCheio()
{
  if(mBoiaNivelAlto == 0 && mBoiaNivelBaixoTanqueFlousing ==1 && machineRun==true )
  {
    Serial.println("Iniciar Floushing");
    mEmFlouching=1;
    desligarSaidas();  
  }
  else if(mBoiaNivelAlto == 0 && mBoiaNivelBaixoTanqueFlousing ==0 && machineRun==true )
  { 
    mEmFlouching=0;
    desligarMaquina();
  } 
}

void sistemaEmBaixaPresao()
{
  if(tipoAlarme5 == true && mBoiaNivelBaixoTanqueFlousing ==1 && machineRun == 0)
  {
    Serial.println("Iniciar Floushing por baixa pressão");
    ligarBombaAltaPressao();  
    mEmFlouching=1;
  }
  else if(tipoAlarme5 == true){ 
    
    mEmFlouching=0;
    Serial.println("Fim Floushing por baixa pressão");
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
  if(mPressostatoNivelAlto==0 && tipoAlarme1==false) 
  {
    alarme=1;
    machine=false;
    machineRun=false;
    tipoAlarme1=true;
    Serial.println("Alarme Pressostato Nivel Alto");
  }else if(mPressostatoNivelAlto==0){
    Serial.println("Alarme Pressostato Nivel Alto");
  } 

  if(mAvariaBombaAltaPressao==1 && tipoAlarme2==false) 
  {
    alarme=1;
    machine=false;
    machineRun=false;
    tipoAlarme2=true;
    Serial.println("Alarme Avaria Bomba Alta Pressao");
  } else if(mAvariaBombaAltaPressao==1){
     Serial.println("Alarme Avaria Bomba Alta Pressao");
  } 

  if( mAvariaBombaDoseadora==0 && tipoAlarme3==false) 
  {
    alarme=1;
    machine=false;
    machineRun=false;
    tipoAlarme3=true;
    Serial.println("Alarme Avaria Bomba Doseadora");
  } else if(mAvariaBombaDoseadora==0){
     Serial.println("Alarme Avaria Bomba Doseadora");
  } 

  if( mReleSequenciaFase==0 && tipoAlarme4==false) 
  {
    alarme=1;
    machine=false;
    machineRun=false;
    tipoAlarme4=true;
    Serial.println("Alarme Rele Sequencia Fase");
  }else if(mReleSequenciaFase==0){
    Serial.println("Alarme Rele Sequencia Fase");
  } 

  if(mPressostatoNivelBaixo==0 && tipoAlarme5==true){
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
   //leitura2 = digitalRead(boiaNivelAlto);                              
  // mBoiaNivelBaixoBruta = digitalRead(boiaNivelBaixoBruta);                     
   mBoiaNivelBaixoTanqueFlousing = digitalRead(boiaNivelBaixoTanqueFloushing);  
   mBoiaNivelAltoTanqueFlousing =digitalRead(boiaNivelAltoTanqueFloushing);
   mPressostatoNivelBaixo =digitalRead(pressostatoNivelBaixo);          
   mPressostatoNivelAlto =digitalRead(pressostatoNivelAlto);           
   mAvariaBombaAltaPressao =digitalRead(avariaBombaAltaPressao);        
   mAvariaBombaDoseadora =digitalRead(avariaBombaDoseadoraRO);         
   mLavagemFiltro=digitalRead(lavagemFiltro);                 
   //mReleSequenciaFase =digitalRead(releSequenciaFase);              
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
    Serial.println("Filtro em lavagem");
     desligarMaquina();
     mLavagemFiltroTermino=1;
  }
}
