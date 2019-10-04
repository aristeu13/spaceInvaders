#include <windows.h>
#include <gl/gl.h>
#include "SOIL.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1

/*
Grupo ALW
Integrantes:
Aristeu Alves Ferreira Neto : 11911BCC027
Lucas Marçal Coutinho       : 11911BCC012
Willy Gabriel Lemos Nery    : 11911BCC009
*/

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

//tipo
struct Aliado{
    float posYinicial,posXinicial,posXmov;
    int keyPressR,keyPressL,vivo,start,partida;
};
struct Tiro{
    float posY[N],posX[N],posXmov[N],posYmov[N];
    double valor[N];
    int desenha;
    //float xPos,xNeg,yPos,yNeg;
};
struct Inimigo{
    float posYmov,posXmov;
    int direcao,naves[5][9],eliminados,teste,tiro,linha;
};

//funcao
void DesenhaNave();
void Atirar();
void LogicaTiro();
void DesenhaInimigo();
static void desenhaSprite(float coluna,float linha, GLuint tex);
static GLuint carregaArqTextura(char *str);
void  carregaTexturas();
void DesenhaTela(int T);
void restart();
void Renderiza(float xPos,float xNeg,float yPos,float yNeg,GLuint img);
void DesenhaTiroInimigo(float xPos, float xNeg, float yPos, float yNeg);
void score();
void animacaoMorte();
//var
struct Aliado aliado;
struct Tiro tiro;
struct Inimigo inimigo;
GLuint tela_2d[4];
GLuint inimigo_2d1;
GLuint inimigo_2d2;
GLuint inimigo_2d3;
GLuint aliado_2d;
GLuint morte_2d1;
GLuint morte_2d2;
GLuint morte_2d3;
GLuint score_2d[10];
int u[5];
GLuint Tscore;

float tiroXposI;
float tiroXnegI;
float tiroYposI;
float tiroYnegI;
float proxNivel;
int delay;
int animacao;
int apenasUmaVez;

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    int i,j;

    //var
    for(i=0;i<5;i++){
        u[i]=0;
    }
    aliado.posXinicial = 0.06;
    aliado.posYinicial = -0.9229;
    aliado.posXmov = 0;
    aliado.keyPressL = 0;
    aliado.keyPressR = 0;
    aliado.vivo = 0;
    aliado.start = 2;
    aliado.partida = 1;

    for(i=0;i<N;i++){
        //tiro.keyPress[i] = 0;
        tiro.valor[i] = 0;
        tiro.posX[i] = 0;
        tiro.posY[i] = 0;
        tiro.posYmov[i] = 0;
        tiro.posXmov[i] = 0;
    }
    tiro.desenha = 1;

    inimigo.teste = 0;
    inimigo.eliminados = 0;
    inimigo.posXmov = 0;
    inimigo.posYmov = 0;
    inimigo.direcao = 1;
    inimigo.linha = 0;
    inimigo.tiro=0;
    for(i=0;i<5;i++){
        for(j=0;j<9;j++){
            inimigo.naves[i][j]=1;

        }
    }

    delay=0;
    proxNivel=0;
    animacao=0;
    apenasUmaVez=1;
    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          800,
                          800,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            if(aliado.start==1){
                Renderiza(1,-1,1,-1,tela_2d[3]);
                Atirar();
                DesenhaNave();
                DesenhaInimigo();
                score();
            }else if(aliado.start==2){
                Renderiza(1,-1,1,-1,tela_2d[0]);
            }else if(aliado.start==3){//ganhou
                Renderiza(1,-1,1,-1,tela_2d[1]);
            }else if(aliado.start==4){//perdeu
                Renderiza(1,-1,1,-1,tela_2d[2]);
                for(i=4;i>=0;i--){
                    Renderiza(0.26161-0.05*i,0.21161-0.05*i,-0.492313,-0.555,score_2d[u[i]]);//MOSTRA O SCORE QUANDO PERDEU
                }
            }
            SwapBuffers(hDC);

            Sleep (1); //se tiver muito rapido sleep(5)
                    //se tiver muito devagar sleep(0.2)
                    //caso contrario manter sleep(1)
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
                case VK_LEFT:
                    aliado.keyPressL = 1;
                break;
                case VK_RIGHT:
                    aliado.keyPressR = 1;
                break;
                case VK_SPACE:
                    LogicaTiro();
                break;
                case '1':
                    if(aliado.start==2){
                        aliado.vivo=1;
                        aliado.start=1;
                    }
                    if(aliado.start==4){
                        aliado.start=-2;
                        restart();
                    }
                break;
                case VK_NUMPAD1:
                    if(aliado.start==2){
                        aliado.vivo=1;
                        aliado.start=1;
                    }

                    if(aliado.start==4){
                        aliado.start=-2;
                        restart();
                    }
                break;
            }
        }
        break;

        case WM_KEYUP:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
                case VK_LEFT:
                    aliado.keyPressL = 0;
                break;
                case VK_RIGHT:
                    aliado.keyPressR = 0;
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);

    carregaTexturas();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

void DesenhaNave(){

    float alturaCanhao = 0.0742;
    float xPos = aliado.posXinicial+aliado.posXmov;
    float xNeg = -aliado.posXinicial+aliado.posXmov;
    float yPos = aliado.posYinicial+alturaCanhao;
    float yNeg = aliado.posYinicial;
    if(aliado.keyPressR == 1 && (aliado.posXinicial+aliado.posXmov)< 1 && animacao==0 ) aliado.posXmov+=0.003165;
    if(aliado.keyPressL == 1 && (-aliado.posXinicial+aliado.posXmov)> -1 && animacao==0 ) aliado.posXmov-=0.003165;

    if(animacao==0)  Renderiza(xPos, xNeg, yPos, yNeg, aliado_2d);
    else{
        tiro.desenha=0;
        glColor3f(0,1,0);
        if(animacao>=0 && animacao<80) Renderiza(xPos, xNeg, yPos, yNeg, morte_2d1);
        else if(animacao>=80 && animacao<160) Renderiza(xPos, xNeg, yPos, yNeg, morte_2d2);
        else if(animacao>=160 && animacao<240) Renderiza(xPos, xNeg, yPos, yNeg, morte_2d3);

        if(animacao>=260){
            aliado.start=4;
            Sleep(500);
        }

        animacao++;

    }
    //}
}

void Atirar(){
    float alturaTiro = 0.0371;
    int i;
    for(i=0;i<N;i++){ //a quantidade de tiros depende de n
        //organizando as variaveis do tiro
        float xPos = tiro.posX[i]+tiro.posXmov[i];
        float xNeg = -tiro.posX[i]+tiro.posXmov[i];
        float yPos = tiro.posY[i]+alturaTiro+tiro.posYmov[i];
        float yNeg = tiro.posY[i]+tiro.posYmov[i];
        //
        tiro.posX[i] = (aliado.posXinicial)/10;//posicao x do tiro antes de atirar
        tiro.posY[i] = aliado.posYinicial+0.0371;////posicao y do tiro antes de atirar

        tiro.posYmov[i]+=tiro.valor[i];

        if((tiro.posYmov[i]+tiro.posY[i])>=1) { //faz o tiro voltar a posicao inicial
                tiro.posYmov[i] = 0;
                tiro.valor[i] = 0;
                tiro.desenha=1;
        }

        if(tiro.posYmov[i]==0) tiro.posXmov[i] = aliado.posXmov;//posicao x do tiro antes de atirar

        if(tiro.desenha){
            glPushMatrix();

            glBegin(GL_QUADS);
                glVertex2f(xPos,yPos);
                glVertex2f(xPos,yNeg);
                glVertex2f(xNeg,yNeg);
                glVertex2f(xNeg,yPos);
                //
            glEnd();
            glPopMatrix();
        }
    }

}

void LogicaTiro(){
    double velTiro = 0.015;
    tiro.valor[0] = velTiro;
}

void DesenhaTiroInimigo(float xPos, float xNeg, float yPos, float yNeg){
    float larguraTiro = 0.044,alturaTiro = 0.0371;
    glPushMatrix();
        glBegin(GL_QUADS);
            glVertex2f(xPos-larguraTiro,yPos-0.1); //direita superior
            glVertex2f(xPos-larguraTiro,yNeg-alturaTiro);//direita inferior
            glVertex2f(xNeg+larguraTiro,yNeg-alturaTiro);//esquerda inferior
            glVertex2f(xNeg+larguraTiro,yPos-0.1);//esquerda superior
        glEnd();
    glPopMatrix();
}

void DesenhaInimigo(){
    float i,j;
    int linhaTiro=0;
    int contadorTiro=0;

    if(inimigo.direcao == 1 && animacao==0){
        inimigo.posXmov += 0.0003+proxNivel;
    }
    if(inimigo.direcao == -1 && animacao==0){
        inimigo.posXmov -= 0.0003+proxNivel;
    }

    for(i=0;i<5;i++){
        for(j=-4;j<5;j++){
            float xPos = 0.05+j/5+inimigo.posXmov;
            float xNeg = -0.05+j/5+inimigo.posXmov;
            float yPos = 0.05+i/5+inimigo.posYmov;
            float yNeg = -0.05+i/5+inimigo.posYmov;
            int l = i, c = j+4;

            if(inimigo.naves[l][c]==1){
                GLuint img_2d;

                    if(i == 0 || i == 1)  img_2d = inimigo_2d1;
                    else if(i == 2 || i == 3) img_2d = inimigo_2d2;
                    else img_2d = inimigo_2d3;
                    glColor3f(1,1,1);
                    Renderiza(xPos, xNeg, yPos, yNeg, img_2d);//renderizando as imagens na tela
                    ////////
                    if(delay>=300 && animacao==0){
                        if(((aliado.posXinicial/10+aliado.posXmov) >= (xNeg - 0.05) && (aliado.posXinicial/10+aliado.posXmov) <= (xPos + 0.05))){
                            if(inimigo.tiro==0){
                                tiroXposI=xPos;
                                tiroXnegI=xNeg;
                                tiroYposI=yPos;
                                tiroYnegI=yNeg;
                                inimigo.tiro=1;
                            }
                        }

                        DesenhaTiroInimigo(tiroXposI, tiroXnegI, tiroYposI, tiroYnegI);

                        float aliadoXpos = aliado.posXinicial+aliado.posXmov;
                        float aliadoXneg = -aliado.posXinicial+aliado.posXmov;
                        float aliadoYneg = aliado.posYinicial;
                        float aliadoYpos = aliado.posYinicial+0.0742;

                        if( ( ( (tiroXnegI+0.044)>=aliadoXneg && (tiroXnegI+0.044)<=aliadoXpos ) || ( (tiroXposI-0.044)>=aliadoXneg && (tiroXposI-0.044)<=aliadoXpos ) ) && ( ( (tiroYnegI-0.0371)>=aliadoYneg && (tiroYnegI-0.0371)<=aliadoYpos ) || ( (tiroXposI-0.1)>=aliadoYneg && (tiroXposI-0.1)<=aliadoYpos ) ) && delay==302){
                            inimigo.tiro=0;
                            animacao=1;
                            delay=303;
                        }
                        if(tiroYnegI<=-1){
                            inimigo.tiro=0;
                        }
                    }
                    /////////

                if(xPos >= 1){
                    inimigo.posYmov -= 0.005;
                    inimigo.direcao = -1;
                }
                if(xNeg <= -1){
                    inimigo.posYmov -= 0.005;
                    inimigo.direcao = 1;
                }
                if(yNeg<=(aliado.posYinicial+0.075)){//quando o inimigo consegue avan�ar
                    aliado.start=4;
                }

                float tiroXpos = tiro.posX[0]+tiro.posXmov[0];
                float tiroXneg = -tiro.posX[0]+tiro.posXmov[0];
                float tiroYpos = tiro.posY[0]+0.0371+tiro.posYmov[0];
                float tiroYneg = tiro.posY[0]+tiro.posYmov[0];
                if( ( (tiroXpos >= xNeg && tiroXpos <= xPos)|| (tiroXneg >= xNeg && tiroXneg <= xPos) ) && ( (tiroYpos >= yNeg && tiroYpos <= yPos) || (tiroYneg>=yNeg && tiroYneg<=yPos ) ) && (tiro.desenha==1) ){
                    inimigo.naves[l][c]=0;
                    tiro.desenha=0;
                }
            }else if(inimigo.naves[l][c]<=0 && inimigo.naves[l][c]>=-120){//animcao da morte dos inimigos
                GLuint img_2d;

                    if(inimigo.naves[l][c]<=0 && inimigo.naves[l][c]>-40) img_2d = morte_2d1;
                    else if(inimigo.naves[l][c]<=-40 && inimigo.naves[l][c]>-80) img_2d = morte_2d2;
                    else img_2d = morte_2d3;

                    glColor3f(1,1,1);
                    Renderiza(xPos, xNeg, yPos, yNeg, img_2d);

                inimigo.naves[l][c]-=1;
            }else if(inimigo.naves[l][c]==-121){ //contando as kills
                inimigo.naves[l][c]-=1;

                if(i == 0 || i == 1)    inimigo.eliminados+=10;
                else if(i == 2 || i == 3) inimigo.eliminados+=20;
                else inimigo.eliminados+=30;

                u[4] = inimigo.eliminados;
                if(inimigo.eliminados>=10000){
                    while(u[4]>9) u[4]=u[4]/10;
                }else u[4] = 0;

                u[3] = inimigo.eliminados;
                if(inimigo.eliminados>=1000){
                    while(u[3]>9) u[3]=u[3]/10;
                }else u[3] = 0;
                u[2] = inimigo.eliminados%1000;
                if(inimigo.eliminados>=100){
                    while(u[2]>9) u[2]=u[2]/10;
                }else u[2] = 0;
                u[1] = inimigo.eliminados%100;
                if(inimigo.eliminados>=10){
                    while(u[1]>9) u[1]=u[1]/10;
                }else u[1] = 0;
                u[0] = inimigo.eliminados%10;
                if(inimigo.eliminados>=1){
                    while(u[0]>9) u[0]=u[0]/10;
                }else u[0] = 0;

                if(inimigo.eliminados>=810*aliado.partida){
                    aliado.start=-1;
                    restart();
                }
                if(inimigo.eliminados>=400*aliado.partida && apenasUmaVez==1){
                    proxNivel=proxNivel+0.0003;
                    apenasUmaVez=0;
                }
                if(inimigo.eliminados>=700*aliado.partida && apenasUmaVez==0){
                    proxNivel=proxNivel+0.001;
                    apenasUmaVez=-1;
                }
            }
        }
    }
    if(aliado.start!=4) tiroYposI-=0.001+proxNivel;
    if(aliado.start!=4) tiroYnegI-=0.001+proxNivel;
    if(delay<=301) delay++;

}

void score(){
    int i;
    Renderiza(-0.6885,-0.97,0.97,0.92,Tscore);
    for(i=4;i>=0;i--){
        Renderiza(-0.40-0.05*i,-0.45-0.05*i,0.97,0.92,score_2d[u[i]]);
    }
}

void Renderiza(float xPos,float xNeg,float yPos,float yNeg,GLuint img_2d){
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, img_2d);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glColor3f(0,0,1);
        glBegin(GL_QUADS);
            glTexCoord2f(1.0, 1.0);
            glVertex2f(xPos,yPos);//canto superior direito
            glTexCoord2f(1.0, 0.0);
            glVertex2f(xPos,yNeg);//cato inferior direiro
            glTexCoord2f(0.0, 0.0);
            glVertex2f(xNeg,yNeg);//canto inferior esquerdo
            glTexCoord2f(0.0, 1.0);
            glVertex2f(xNeg,yPos);//canto superior direito
        glEnd();
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void carregaTexturas(){
    char str[50];
    int i;

    for(i=0;i<3;i++){
        sprintf(str,"assets/tela%d.png",i);
        tela_2d[i] = carregaArqTextura(str);
    }

    sprintf(str,"assets/fundo.png");
    tela_2d[3] = carregaArqTextura(str);

    sprintf(str,"assets/inimigo1.png");
    inimigo_2d1 = carregaArqTextura(str);

    sprintf(str,"assets/inimigo2.png");
    inimigo_2d2 = carregaArqTextura(str);

    sprintf(str,"assets/inimigo3.png");
    inimigo_2d3 = carregaArqTextura(str);

    sprintf(str,"assets/Laser_Cannon.png");
    aliado_2d = carregaArqTextura(str);

    sprintf(str,"assets/morte1.png");
    morte_2d1 = carregaArqTextura(str);

    sprintf(str,"assets/morte2.png");
    morte_2d2 = carregaArqTextura(str);

    sprintf(str,"assets/morte3.png");
    morte_2d3 = carregaArqTextura(str);

    for(i=0;i<10;i++){
        sprintf(str,"assets/%d.png",i);
        score_2d[i] = carregaArqTextura(str);
    }

    sprintf(str,"assets/score.png");
    Tscore = carregaArqTextura(str);
}

static GLuint carregaArqTextura(char *str){
    // http://www.lonesock.net/soil.html
    GLuint tex = SOIL_load_OGL_texture
        (
            str,
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y |
            SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
        );

    if(0 == tex){
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }

    return tex;
}

void restart(){
    int i,j;
    inimigo.tiro=0;
    delay=0;
    animacao=0;
    apenasUmaVez=1;
    proxNivel-=0.0003;
    proxNivel=proxNivel-0.001;

    aliado.posXinicial = 0.06;
    aliado.posYinicial = -0.9229;
    //aliado.posXmov = 0;
    aliado.keyPressL = 0;
    aliado.keyPressR = 0;
    aliado.vivo = 1;

    for(i=0;i<N;i++){
        //tiro.keyPress[i] = 0;
        tiro.valor[i] = 0;
        tiro.posX[i] = 0;
        tiro.posY[i] = 0;
        tiro.posYmov[i] = 0;
        tiro.posXmov[i] = 0;
    }
    tiro.desenha=1;

    inimigo.posXmov = 0;
    inimigo.posYmov = 0;
    inimigo.direcao = 1;
    for(i=0;i<5;i++){
        for(j=0;j<9;j++){
            inimigo.naves[i][j]=1;
        }
    }
    if(aliado.start==-1){//quando ganha
        proxNivel+=0.0001;
        aliado.partida+=1;
        inimigo.posYmov -= 0.1;//desce uma linha
        aliado.start=1;//recomeca o jogo na hora
        printf("%f\n",proxNivel);
    }else if(aliado.start==-2){//quando perde
        proxNivel=0;
        inimigo.eliminados = 0;
        aliado.partida=1;
        aliado.start=1;
        for(i=0;i<5;i++){
            u[i]=0;
        }
    }

}
