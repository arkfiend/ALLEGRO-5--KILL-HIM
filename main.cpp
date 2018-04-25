#include "dll.c"       // Inclui todas as bibliotecas necessárias 
#define xTela 800      // Define o tamanho máximo da tela "X"
#define yTela 600      // Define o tamanho máximo da tela "Y"
#define vidaMax 10.0      // Define o número máximo de vidas inicial.
#define fps 60         // Define o número de frames por segundo do jogo.


void jogo();
void menu();
void insereRecord(int);


//////////////////////////// ALEATORIO
int aleatorio(int fim){           //
    int ini=50;                   //
	int r= rand() % (fim-ini);    //
	r=r+ini;                      //
	return r;                     //
}                                 //
////////////////////////////////////





/////////// DESENHA UMA MATRIZ CONTENDO O ALFABETO ////////////////////////////////////////////////////
char retornaLetra(int x, int y){                                                                     // 
  //Cria matrix 3x9                                                                                  //
  char alfa[3][9];                                                                                   //
  alfa[0][0]='A'; alfa[0][1]='B'; alfa[0][2]='C'; alfa[0][3]='D'; alfa[0][4]='E'; alfa[0][5]='F';    //
  alfa[0][6]='G'; alfa[0][7]='H'; alfa[0][8]='I'; alfa[1][0]='J'; alfa[1][1]='K'; alfa[1][2]='L';    //
  alfa[1][3]='M'; alfa[1][4]='N'; alfa[1][5]='O'; alfa[1][6]='P'; alfa[1][7]='Q'; alfa[1][8]='R';    //
  alfa[2][0]='S'; alfa[2][1]='T'; alfa[2][2]='U'; alfa[2][3]='V'; alfa[2][4]='W'; alfa[2][5]='X';    //
  alfa[2][6]='Y'; alfa[2][7]='Z'; alfa[2][8]='-';                                                    //
  return alfa[x][y];                                                                                 //
}                                                                                                    //
///////////////////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////////
void menu(){
  ALLEGRO_BITMAP *back_menu = al_load_bitmap("db//pic//back_menu.png");
  ALLEGRO_BITMAP *back_info = al_load_bitmap("db/pic//info.png");
  ALLEGRO_BITMAP *back_record = al_load_bitmap("db//pic//back_record.png");
  ALLEGRO_FONT *limbo = al_load_ttf_font("db//font//limbo.ttf", 80, 0);
  int x=1; // Variável do loop
  int pos_x=0; // Posição x do mouse
  int pos_y=0; // Posição y do mouse
  
  // SONS - SAMPLES
  ALLEGRO_SAMPLE *music_intro = al_load_sample("db//sound//music_intro.wav"); // Carrego a musica
  ALLEGRO_SAMPLE *botao_inicio = al_load_sample("db//sound//hit_01.wav");
  
  // SONS - INSTANCES
  ALLEGRO_SAMPLE_INSTANCE *inst_music_intro =  al_create_sample_instance(music_intro); // Cria a instancia
  ALLEGRO_SAMPLE_INSTANCE *inst_botao_inicio = al_create_sample_instance(botao_inicio);
  
  // SONS - MIXERS
  al_attach_sample_instance_to_mixer(inst_music_intro, al_get_default_mixer()); // cria  o mixer
  al_attach_sample_instance_to_mixer(inst_botao_inicio, al_get_default_mixer());
  
  //SONS - PLAYMODES
  al_set_sample_instance_playmode(inst_music_intro, ALLEGRO_PLAYMODE_LOOP); //apenas para musicas e loop
  al_set_sample_instance_playmode(inst_botao_inicio, ALLEGRO_PLAYMODE_ONCE); //apenas uma vez
  
  //SONS - GANHO
  al_set_sample_instance_gain(inst_music_intro, 0.8); // volume.   
  al_set_sample_instance_gain(inst_botao_inicio, 2.0); //volume
  
  
  ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();          // Cria fila de eventos
  ALLEGRO_EVENT eventoAtual;
  al_register_event_source(fila, al_get_keyboard_event_source()); // Adapta o uso do teclado
  al_register_event_source(fila, al_get_mouse_event_source()); // Adapta o uso do mouse
  
  FILE *rec=fopen("record.txt", "r"); char n[3]; int record; fscanf(rec, "%c%c%c %d", &n[0], &n[1], &n[2], &record); fclose(rec); //pega record
  
  al_draw_bitmap(back_menu, 0, 0, 0);
  al_flip_display();
  al_stop_samples(); //para todos os sons
  al_play_sample_instance(inst_music_intro);  // Toca o som
  
  //LOOP
  while(x==1){
    al_wait_for_event(fila, &eventoAtual);
    if (eventoAtual.type==ALLEGRO_EVENT_KEY_DOWN){
      if (eventoAtual.keyboard.keycode==ALLEGRO_KEY_ESCAPE){ exit(1);}
    }
    else if(eventoAtual.type==ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
      pos_x=eventoAtual.mouse.x;
      pos_y=eventoAtual.mouse.y;
      
      //Cadastro dos locais dos botões
      
      // INICIAR
      if (pos_x>=280 && pos_x<=500   && pos_y>=4 && pos_y<=102){
        al_stop_sample_instance(inst_music_intro);
        al_play_sample_instance(inst_botao_inicio);
        jogo();
      }
      
      // RECORD
      else if(pos_x>=575 && pos_y<=795  &&  pos_y>=100 && pos_y<=202){
        al_play_sample_instance(inst_botao_inicio);
        al_draw_bitmap(back_record, 0, 0, 0);
        al_draw_textf(limbo, al_map_rgb(0,0,0), 400,  365, ALLEGRO_ALIGN_CENTRE,"%c%c%c", n[0], n[1], n[2]); 
        al_draw_textf(limbo, al_map_rgb(0,0,0), 400,  465, ALLEGRO_ALIGN_CENTRE,"%d", record); 
        al_flip_display();
        al_rest(5.0);
        al_draw_bitmap(back_menu, 0, 0, 0);
        al_flip_display();        
      }
      
      // INFO
      else if(pos_x>=2 && pos_x<=227  && pos_y>=254 && pos_y<=353){
        al_play_sample_instance(inst_botao_inicio);
        al_draw_bitmap(back_info, 0, 0, 0);
        al_flip_display();
        al_rest(5.0);
        al_draw_bitmap(back_menu, 0, 0, 0);
        al_flip_display();
      }
      
    }// Fim do loop
    
    
    
  }
  
}






///////////////////////// TELA PARA INSERÇÃO DO NOVO NOME ////////////////////////////////////////////////////////////////////////////////
void insereRecord(int novoRecord){
  ALLEGRO_FONT *font2 = al_load_ttf_font("db//font//limbo.ttf", 50, 0); // Fonte para visualização das letras
  char novo[3]; novo[0]='-'; novo[1]='-'; novo[2]='-';  // Vetor contendo cada letra do novo nome do record
  int proximo=0;                                        // variável do loop, qque vai rodar 3 vezes
  al_clear_to_color(al_map_rgb(0,0,0)); // Fundo preto 
  ALLEGRO_BITMAP *alph= al_load_bitmap("db//pic//alfa.png"); // Foto da matriz do alfabeto
  ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();          // Cria fila de eventos
  al_register_event_source (fila, al_get_mouse_event_source()); // Fila pega eventos do mouse

  // SONS - SAMPLES
  ALLEGRO_SAMPLE *music_record = al_load_sample("db//sound//music_record.ogg");
  ALLEGRO_SAMPLE *hit_alph = al_load_sample("db//sound//hit_alph.wav");
  ALLEGRO_SAMPLE *hit_error = al_load_sample("db//sound//hit_error.wav");
  
  // SONS - INSTANCIAS
  ALLEGRO_SAMPLE_INSTANCE *inst_music_record = al_create_sample_instance(music_record);
  ALLEGRO_SAMPLE_INSTANCE *inst_hit_alph = al_create_sample_instance(hit_alph);
  ALLEGRO_SAMPLE_INSTANCE *inst_hit_error = al_create_sample_instance(hit_error);
  
  // SONS - MIXERS
  al_attach_sample_instance_to_mixer(inst_music_record, al_get_default_mixer());
  al_attach_sample_instance_to_mixer(inst_hit_alph, al_get_default_mixer());
  al_attach_sample_instance_to_mixer(inst_hit_error, al_get_default_mixer());
  
  // SONS - PLAYMODE
  al_set_sample_instance_playmode(inst_music_record, ALLEGRO_PLAYMODE_LOOP);
  al_set_sample_instance_playmode(inst_hit_alph, ALLEGRO_PLAYMODE_ONCE);
  al_set_sample_instance_playmode(inst_hit_error, ALLEGRO_PLAYMODE_ONCE);
  
  // SONS - GANHO
  al_set_sample_instance_gain(inst_music_record, 0.8);
  al_set_sample_instance_gain(inst_hit_alph, 0.8);
  al_set_sample_instance_gain(inst_hit_error, 0.8);

  // DRAW
  al_draw_textf(font2, al_map_rgb(255,255,255), xTela/2, 500, ALLEGRO_ALIGN_CENTRE, "%c %c %c", novo[0], novo[1], novo[2]); // Draw
  al_draw_bitmap(alph, 0, 0, 0);                                                                                            // Draw
  
  int x=0;  // Posição x do mouse
  int y=0;  // Posição y do mouse
  ALLEGRO_EVENT eventoAtual; // Criação do evento de escuta
  
  //////////// LOOP ///////////////
  while(proximo<3){
    
    al_wait_for_event(fila, &eventoAtual);
    if (eventoAtual.type==ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){ //Ao clicaar no mouse
      // X- 810 / 9 = 90
      // Y- 345 / 3 = 115
      x = eventoAtual.mouse.x/90;  // /90, devido ao local e tamanho onde a foto fica na tela
      y = eventoAtual.mouse.y/115; // /115, devido ao local e tamanho one a foto fica na tela
      
      novo[proximo]=retornaLetra(y, x); //o contrario, pq sim!
      //Se y é maior que 2, significa que clicou em lugar errado, sem letras!
      if(novo[proximo]!='-' && y<=2){ //Se for diferente de traço, significa que é para ser adicionado
        al_clear_to_color(al_map_rgb(0,0,0)); // Fundo preto
        al_draw_bitmap(alph, 0, 0, 0); // Todas as letras
        al_draw_textf(font2, al_map_rgb(255,255,255), xTela/2, 500, ALLEGRO_ALIGN_CENTRE, "%c %c %c", novo[0], novo[1], novo[2]);
        al_stop_sample_instance(inst_hit_alph);  // som
        al_stop_sample_instance(inst_hit_error); // som
        al_play_sample_instance(inst_hit_alph);  // som
        proximo++;
      }else if(x==8 && y==2){ // Se clicou no traço
        novo[0]='-'; novo[1]= '-'; novo[2]='-'; //Zero tudo 
        al_clear_to_color(al_map_rgb(0,0,0)); // Fundo preto
        al_draw_bitmap(alph, 0, 0, 0); // Todas as letras
        al_draw_textf(font2, al_map_rgb(255,255,255), xTela/2, 500, ALLEGRO_ALIGN_CENTRE, "%c %c %c", novo[0], novo[1], novo[2]);
        proximo=0; // Reinicio a contagem  
        al_stop_sample_instance(inst_hit_alph);  // som
        al_stop_sample_instance(inst_hit_error); // som
        al_play_sample_instance(inst_hit_error); // som
      }
    }
    
    al_flip_display(); // Desenho na tela
  }
  
  // Com o nome, basta eu recolocar em um novo arquivo de texto o record.
  FILE *arq=fopen ("record.txt", "w");
  
  fprintf(arq, "%c%c%c %d", novo[0], novo[1], novo[2], novoRecord);
  fclose(arq);
  
  return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////// TELA DO JOGO ////////////////////
void jogo(){
  FILE *rec=fopen("record.txt", "r");
  
  char nome[30]; int record;
  fscanf(rec, "%s %d", &nome, &record);
  fclose(rec);
  
  /////////////////////////////////// LISTA DE INIMIGOS  ////////////////
  int tamX[2];                                                         //
  int tamY[2];                                                         //
                                                                       //
  ALLEGRO_BITMAP *enemy_00 = al_load_bitmap("db//pic//enemy_00.png");  // Foto do inimigo 00
  tamX[0]=114;  tamY[0]=72;  // Medidas do inimigo 00                  //
                                                                       //
  ALLEGRO_BITMAP *enemy_01 = al_load_bitmap("db//pic//enemy_01.png");  // Foto do inimigo 01
  tamX[1]=76;   tamY[1]=72; // Medidas do inimigo 01                   //
                                                                       //  
  /////////////////////////////////// LISTA DE INIMIGOS  ////////////////
  
  /////////////////////////////////////////////////////////////////////////////////////////////////// FOTOS DIVERSAS
                                                                                                            //
  ALLEGRO_BITMAP *back1 = al_load_bitmap("db//pic//back1.png");  // Fundo da tela                           //
  al_convert_mask_to_alpha(back1, al_map_rgb(255, 0, 255));      // Conversão para usar partes do fundo     //
                                                                                                            //
  ALLEGRO_BITMAP *tarja = al_load_bitmap("db//pic//tmp.png");    // Tarja preta onde fica a VIDA            //
  al_convert_mask_to_alpha(tarja, al_map_rgb(255, 0, 255));      // Conversão da foto "tarja"               //
                                                                                                            //
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////


  ////////////////////////////////////////////// VARIAVEIS ALLEGRO /////////////////////////////////
                                                                                                  //
  ALLEGRO_TIMER *tempo  = al_create_timer(1.0/fps);              // FPS                           //
                                                                                                  //
  //////////////////////////////////////////////////////////////////////////////////////////////////
  
  
  /////////////////////////////////////////// SONS /////////////////////////////////////////////////////////////////////
    // SONS - SAMPLES
  ALLEGRO_SAMPLE *music_game = al_load_sample("db//sound//music_game.wav"); // Musica do jogo
  ALLEGRO_SAMPLE *hit_00 = al_load_sample("db//sound//hit_02.wav"); //hit do mouse
  ALLEGRO_SAMPLE *boss_00_view = al_load_sample("db//sound//boss_00_view.wav");
  ALLEGRO_SAMPLE *boss_00_die = al_load_sample("db//sound//boss_00_die.wav");
  ALLEGRO_SAMPLE *win_00 = al_load_sample("db//sound//win_00.wav");
  
  // SONS - INSTANCES
  ALLEGRO_SAMPLE_INSTANCE *inst_music_game =  al_create_sample_instance(music_game); // Cria a instancia
  ALLEGRO_SAMPLE_INSTANCE *inst_hit_00 = al_create_sample_instance(hit_00);
  ALLEGRO_SAMPLE_INSTANCE *inst_boss_00_view = al_create_sample_instance(boss_00_view);
  ALLEGRO_SAMPLE_INSTANCE *inst_boss_00_die = al_create_sample_instance(boss_00_die);
  ALLEGRO_SAMPLE_INSTANCE *inst_win_00 = al_create_sample_instance(win_00);
  
  // SONS - MIXERS
  al_attach_sample_instance_to_mixer(inst_music_game, al_get_default_mixer()); // cria  o mixer
  al_attach_sample_instance_to_mixer(inst_hit_00, al_get_default_mixer());
  al_attach_sample_instance_to_mixer(inst_boss_00_view, al_get_default_mixer());
  al_attach_sample_instance_to_mixer(inst_boss_00_die, al_get_default_mixer());
  al_attach_sample_instance_to_mixer(inst_win_00, al_get_default_mixer());
  
  //SONS - PLAYMODES
  al_set_sample_instance_playmode(inst_music_game, ALLEGRO_PLAYMODE_LOOP); //apenas para musicas e loop
  al_set_sample_instance_playmode(inst_hit_00, ALLEGRO_PLAYMODE_ONCE); //apenas uma vez
  al_set_sample_instance_playmode(inst_boss_00_view, ALLEGRO_PLAYMODE_ONCE); //apenas uma vez
  al_set_sample_instance_playmode(inst_boss_00_die, ALLEGRO_PLAYMODE_ONCE); //apenas uma vez
  al_set_sample_instance_playmode(inst_win_00, ALLEGRO_PLAYMODE_ONCE); //apenas uma vez
  
  //SONS - GANHO
  al_set_sample_instance_gain(inst_music_game, 0.8); // volume.  
  al_set_sample_instance_gain(inst_hit_00, 0.8); // volume.  
  al_set_sample_instance_gain(inst_boss_00_die, 0.8); // volume.  
  al_set_sample_instance_gain(inst_boss_00_view, 0.8); // volume.  
  al_set_sample_instance_gain(inst_win_00, 0.8); // volume.  
  
  /////////////////////////////////////////// SONS /////////////////////////////////////////////////////////////////////
  
  
  float tempoDeVida=vidaMax;  // Tempo para o jogo acabar
  int aparece=1;       // Identifica se é hora de mostrar o inimigo ou não
  int acertos=-1;       // Mostra quantas vezes acertou um inimigo

  
  int numEnemy=0; //Indica o numero do inimigo atual.
  int enemy_x=aleatorio(xTela-tamX[numEnemy]);  //Primeira posição X do inimigo
  int enemy_y=aleatorio(yTela-tamY[numEnemy]);  // Primeira posição Y do inimigo
  int delay=0; //Delay de frames, para conta dos 60 fps
  
  int mouseX=0; // X inicial do mouse
  int mouseY=0; // Y inicial do mouse
  

  ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();          // Cria fila de eventos
  al_register_event_source (fila, al_get_mouse_event_source()); // Fila pega eventos do mouse
  al_register_event_source (fila, al_get_keyboard_event_source()); // Fila pega eventos do teclado
  al_register_event_source(fila, al_get_timer_event_source(tempo)); 
  
  ALLEGRO_FONT *font = al_load_ttf_font("db//font//arial.ttf", 20, 0); // Cria a fonte para exibição
  
  int x=1; // Variável do loop
  
  ///////////////////////////////////////////////////////////////////////////////
  // 2 fundos iguais
  //2 fundos diferentes
  // fundo apenas no 60 fps
  // regiao do fundo --- DEU CERTO CARALHOOOOOOOOOOOOOOOOOO
  
  al_draw_bitmap(tarja, 5, 5, 0);
  al_draw_bitmap_region(back1,  0,     0,     800,    600,    0,     0,    0);      // Fundo
  // Nome do sprite--------|    |      |        |      |      |      |     |
  // inicio X do sprite---------|      |        |      |      |      |     |
  // inicio Y do sprite----------------|        |      |      |      |     |
  // Tamanho do sprite em X --------------- ----|      |      |      |     |
  // fim Y do sprite-----------------------------------|      |      |     |
  // X onde vai ficar na tela---------------------------------|      |     |
  // Y onde vai ficar na tela----------------------------------------|     |
  // Girar foto------------------------------------------------------------|
      
  
 //Mostrar dados do recordista
 al_draw_bitmap(tarja, 5, 200, 0);  
 al_draw_textf(font, al_map_rgb(255,255,255), 7, 175,ALLEGRO_ALIGN_LEFT, "RECORD");      
 al_draw_textf(font, al_map_rgb(255,255,255), 50, 202,ALLEGRO_ALIGN_CENTRE, "%s", nome);
 al_draw_textf(font, al_map_rgb(255,255,255), 50, 222,ALLEGRO_ALIGN_CENTRE, "%d", record);
 
 al_play_sample_instance(inst_music_game); //toca a musica do jogo
 
 al_start_timer(tempo); // Inicia o contador de FPS
  
  while (x){
    ALLEGRO_EVENT eventoAtual;
    al_wait_for_event(fila, &eventoAtual);
    if (eventoAtual.type==ALLEGRO_EVENT_TIMER){
      //A cada loop, diminuo 1 até dar 6, onde diminuo 1 da vida.
      delay++;  if (delay==60){tempoDeVida=tempoDeVida-1; delay=0;} //Diminui vidas
      
      // Caso o player tenha acertado o inimigo:
      if (aparece==1){
        al_stop_sample_instance(inst_boss_00_die); // parar o audio de kill
        al_play_sample_instance(inst_boss_00_die); // tocar audio de kill
        al_play_sample_instance(inst_boss_00_view);
        
        tempoDeVida=tempoDeVida+0.2;
        acertos++;
        //Tenho que tratar o fundo aqui antes de tratar o novo aleatorio, pra tampar a  parada
        al_draw_bitmap_region(back1,  enemy_x,   enemy_y,   tamX[0],    tamY[0],    enemy_x,     enemy_y,    0);      // Fundo
        // Nome do sprite--------|      |           |        |            |           |            |         |
        // inicio X do sprite-----------|           |        |            |           |            |         |
        // inicio Y do sprite-----------------------|        |            |           |            |         |
        // Tamanho do sprite em X ---------------------------|            |           |            |         |
        // fim Y do sprite------------------------------------------------|           |            |         |
        // X onde vai ficar na tela---------------------------------------------------|            |         |
        // Y onde vai ficar na tela----------------------------------------------------------------|         |
        // Girar foto----------------------------------------------------------------------------------------|
        
        //Calculo um novo x e y para o inimigo
        enemy_x=0; // tmp //50 de segurança!
        while (enemy_x<=5+49+50){ enemy_x=aleatorio(xTela-tamX[numEnemy]); } // garante que nao vai desenhar nos templates pretos.
        enemy_y=aleatorio(yTela-tamY[numEnemy]);  // Novo Y para o inimigo
      }
        
      //
      al_draw_bitmap(enemy_00, enemy_x, enemy_y, 0); // Desenho o inimigo na nova posição
      aparece=0; // Não tem necessidade de printar de novo
       
      /////////////////////// P R I N T s /////////////////////////////////////////////////////////
      al_draw_bitmap(tarja, 5, 5, 0);                                                            //
      al_draw_textf(font, al_map_rgb(255,255,255), 13, 10,ALLEGRO_ALIGN_LEFT, "TEMPO");          //
      al_draw_textf(font, al_map_rgb(255,255,255), 28, 30,ALLEGRO_ALIGN_LEFT, "%.0f", tempoDeVida);     //
      al_draw_bitmap(tarja, 5, 100, 0);                                                          //
      al_draw_textf(font, al_map_rgb(255,255,255), 25, 102,ALLEGRO_ALIGN_LEFT, "HITS");          //
      al_draw_textf(font, al_map_rgb(255,255,255), 40, 122,ALLEGRO_ALIGN_CENTRE, "%d", acertos); //      
      /////////////////////////////////////////////////////////////////////////////////////////////
      
      al_flip_display(); // Mostra a tela
    }
    
    // ESC = FECHA O PROGRAMA
    else if (eventoAtual.type==ALLEGRO_EVENT_KEY_DOWN){
      if (eventoAtual.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
        al_stop_sample_instance(inst_music_game); // para a musica dessa tela
        x=0;
        acertos=0; // para que nao seja cotado o record.
      }
    }
    
    //Clique do mouse 
    else if (eventoAtual.type==ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
      al_play_sample_instance(inst_hit_00);
      // Cliquei no alvo correto
      if (eventoAtual.mouse.x >= enemy_x && eventoAtual.mouse.x <= enemy_x+tamX[numEnemy]){
        if (eventoAtual.mouse.y >= enemy_y && eventoAtual.mouse.y <= enemy_y+tamY[numEnemy]){
          aparece=1;
        }
      }
    }
   
   if (tempoDeVida<=0.0){
     al_stop_sample_instance(inst_music_game); // para a musica dessa tela
     //Ver se bateu record
     if (acertos>record){
       al_play_sample_instance(inst_win_00);
       insereRecord(acertos);
     }
   x=0;
   } // Tempo de jogo acabou
  
  }// fim do loop
  al_stop_sample_instance(inst_music_game); //para todos os sons
  menu();
}





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////











//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(){
  al_init();                    // Inicializa os addons                 
  al_install_mouse();           // Habilita a interação com o mouse    
  al_init_image_addon();        // Habilita adição de imagens
  al_init_font_addon();         // Habilita a imortação de fontes
  al_init_ttf_addon();          // Habilita o uso de fontes no formato TTF
  al_install_keyboard();        // Habilita o uso de teclado
  al_install_audio();           // Habilita o áudio
  al_init_acodec_addon();       // Habilita o acodec
  al_reserve_samples(50);       // Numero de arquivos de son que serão utilizados no jogo
  
  srand((unsigned)time(NULL));  // Random
  ALLEGRO_DISPLAY *display = al_create_display(xTela, yTela);    // Cria tela 800x600             
  menu();
  return 0;
}






