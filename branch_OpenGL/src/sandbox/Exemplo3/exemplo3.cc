#include "../../framework/engine.h"
#include "../../framework/scene.h"
#include "../../framework/image.h"
#include "../../framework/sprite.h"
#include "../../framework/videomanager.h"
#include "../../framework/inputmanager.h"
#include "../../framework/timehandler.h"
#include "../../framework/vector2D.h"
#include <cmath>

//http://gpwiki.org/index.php/SDL:Tutorials:Practical_Keyboard_Input

using namespace std;
using namespace framework;


class Projetil : public Sprite{
  public:
    ::std::list<Vector2D> trajetoria;
    Animation *going_up;
    Vector2D origem;
    Projetil( Image* imagem, Vector2D pos, Vector2D frameSz ) {
      this->Initialize( imagem );
      this->set_position( pos );
      
      this->image()->set_frame_size(frameSz);
      going_up = new Animation(0, 0, -1);

    }
    void calculaReta( Vector2D A, Vector2D B ){
      double slope;
      
//      this->origem = A;
      Vector2D Ponto( A.x, A.y );
      
      slope = ( B.y - A.y ) / ( B.x - A.x ); // a boa e velha equacao da reta
      
      for( int i = (int) A.x; i < (int) A.x + B.x; i++ ){
        Ponto.x = i;
        Ponto.y = slope * Ponto.x + A.y;
        trajetoria.push_front(Ponto);
      
      }

    }
    void Rush()
    {
      this->SelectAnimation(going_up);
      set_position(origem);
      Vector2D position;
//    Vector2D position(this->position().x, this->position().y);

      printf("------------------------\n");  
      list<Vector2D>::iterator it = trajetoria.begin();// .end();
      while (it != trajetoria.end()) {
        position.x = it->x;
        position.y = it->y;
        ++it;
        trajetoria.pop_front();
        set_position(position);
//        trajetoria.erase(it);
//Update(1.3);
      printf( "%lf %lf\n", position.x, position.y );
      }

//      is_walking_ = false;
    }
  
};

class Mage : public Sprite {
  public:
    int direction_;
    float speed;
//    void setFrameSize(Vector2D newFrameSize) {
//      Vector2D frameSize;
//      frameSize = newFrameSize;
//      this->image()->set_frame_size(frameSize);
//    };
    Projetil *magia;
    //Mage::Mage(string imagem, Vector2D pos); Vector2D(280, 210)
    Mage(Image* imagem, Vector2D pos, Vector2D frameSz ){
      this->Initialize( imagem );
      this->set_position( pos );      
//      this->magia = proj;
//        setFrameSize( frameSz );
      this->image()->set_frame_size( frameSz );
        standing_up = new Animation(0, 0, -1);
        standing_up_right = new Animation(0, 1, -1);
        standing_right =  new Animation(0, 2, -1);
//        standing_right =  new Animation(0, 4, 5,6,7,8 -1);
        standing_down_right =  new Animation(0, 3, -1);
        standing_down =  new Animation(0, 4, -1);
        standing_down_left = new Animation(0, 6, -1);
        standing_left = new Animation(0, 7, -1);
        standing_up_left = new Animation(0, 8, -1);
 
        walking_up = new Animation(4, 10, 20, 30, 40, -1);
        walking_up_right = new Animation(4, 11, 21, 31, 41, -1);
        walking_right = new Animation(4, 12, 22, 32, 42, -1);
        walking_down_right = new Animation(4, 13, 23, 33, 43, -1);
        walking_down = new Animation(4, 14, 24, 34, 44, -1);
        walking_down_left = new Animation(4, 16, 26, 36, 46, -1);
        walking_left = new Animation(4, 17, 27, 37, 47, -1);
        walking_up_left = new Animation(4, 18, 28, 38, 48, -1);

        spell_animation_up =  new Animation(4, 50, 60, 70, 80, -1);
        spell_animation_up_right =  new Animation(4, 51, 61, 71, 81, -1);
        spell_animation_right =  new Animation(4, 52, 62, 72, 82, -1);
        spell_animation_down_right =  new Animation(4, 53, 63, 73, 83, -1);
        spell_animation_down =  new Animation(4, 54, 64, 74, 84, -1);
        spell_animation_down_left =  new Animation(4, 56, 66, 76, 86, -1);
        spell_animation_left =  new Animation(4, 57, 67, 77, 87, -1);
        spell_animation_up_left =  new Animation(4, 58, 68, 78, 88, -1);

        direction_ = DOWN;
        is_walking_ = false;
        speed = 5;
        Stand();

    }
    enum dir {UP,UP_LEFT,LEFT,DOWN_LEFT,DOWN,DOWN_RIGHT,RIGHT,UP_RIGHT};
    void Stand() {
//        is_walking_ = false;
//        Vector2D frameSize(74, 60);
//Vector2D frameSize(74, 61)
//        this->image()->set_frame_size(frameSize);
        switch(direction_)
        {
          case UP:
            this->SelectAnimation(standing_up);
            break;
          case UP_RIGHT:
            this->SelectAnimation(standing_up_right);
            break;
          case RIGHT:
            this->SelectAnimation(standing_right);
            break;
          case DOWN_RIGHT:
            this->SelectAnimation(standing_down_right);
            break;
          case DOWN:
            this->SelectAnimation(standing_down);
            break;
          case DOWN_LEFT:
            this->SelectAnimation(standing_down_left);
            break;
          case LEFT:
//            this->set_mirror(Image::MIRROR_HFLIP);
//            this->set_visible(false);
//        this->image()->MIRROR_HFLIP;
            this->SelectAnimation(standing_left);
            break;
          case UP_LEFT:
//            this->set_mirror(Image::MIRROR_HFLIP);
//            this->set_visible(false);
//        this->image()->MIRROR_HFLIP;
            this->SelectAnimation(standing_up_left);
            break;
          default:
            break;
          
          }
//        if (direction_ == UP) {
//        } else if (direction_ == 1) {
//        } else if (direction_ == 2) {
//        } else {
//        }
    }

    void Move() {

        Vector2D position(this->position().x, this->position().y);
        if (is_walking_){
          switch(direction_)
          {
            case UP:
                position.y -= speed;
            break;
            case UP_RIGHT:
                position.y -= speed;
                position.x += speed;
                break;
            case RIGHT:
                position.x += speed;
                break;
            case DOWN_RIGHT:
                position.y += speed;
                position.x += speed;
                break;
            case DOWN:
                position.y += speed;
                break;
            case DOWN_LEFT:
                position.y += speed;
                position.x -= speed;
                break;
            case LEFT:
                position.x -= speed;
                break;
            case UP_LEFT:
                position.y -= speed;
                position.x -= speed;
                break;
            default:
                break;
          }
        }else
          Stand();
      set_position(position);
//      is_walking_ = false;
    }

    void Walk() {
//        is_walking_ = true;
//        Vector2D frameSize(74, 60);
//        Vector2D frameSize(74, 61);

//        this->image()->set_frame_size(frameSize);
        switch(direction_)
        {
          case UP:
            this->SelectAnimation(walking_up);
            break;
          case UP_RIGHT:
            this->SelectAnimation(walking_up_right);
            break;
          case RIGHT:
            this->SelectAnimation(walking_right);
            break;
          case DOWN_RIGHT:
            this->SelectAnimation(walking_down_right);
            break;
          case DOWN:
            this->SelectAnimation(walking_down);
            break;
          case DOWN_LEFT:
            this->SelectAnimation(walking_down_left);
            break;
          case LEFT:
            this->SelectAnimation(walking_left);
            break;
          case UP_LEFT:
            this->SelectAnimation(walking_up_left);
            break;
          default:
            break;
    }
//            is_walking_ = false;
    Move();
  }

    void Spell( Vector2D destino ) {
//    void Spell(  ) {
//        Vector2D frameSize(74, 60);
//        Vector2D frameSize(69, 57);
//        this->image()->set_frame_size(frameSize);
//        this->SelectAnimation(spell_animation_up);
       this->magia->calculaReta( framework::Vector2D( this->position().x, this->position().y) , destino );
       this->magia->Rush();
       
        switch(direction_)
        {
          case UP:
            this->SelectAnimation(spell_animation_up);
//            this->SelectAnimation(standing_up);
            break;
          case UP_RIGHT:
            this->SelectAnimation(spell_animation_up_right);
            break;
          case RIGHT:
            this->SelectAnimation(spell_animation_right);
            break;
          case DOWN_RIGHT:
            this->SelectAnimation(spell_animation_down_right);
            break;
          case DOWN:
            this->SelectAnimation(spell_animation_down);
            break;
          case DOWN_LEFT:
            this->SelectAnimation(spell_animation_down_left);
            break;
          case LEFT:
            this->SelectAnimation(spell_animation_left);
            break;
          case UP_LEFT:
            this->SelectAnimation(spell_animation_up_left);
            break;
          default:
            break;
//                        Stand();
       }
    }

    bool is_walking_;


  private:
    Animation *standing_down;
    Animation *standing_down_right;
    Animation *standing_right;
    Animation *standing_up_right;
    Animation *standing_up;
    Animation *standing_up_left;
    Animation *standing_left;
    Animation *standing_down_left;

    Animation *walking_up;
    Animation *walking_up_right;
    Animation *walking_right;
    Animation *walking_down_right;
    Animation *walking_down;
    Animation *walking_down_left;
    Animation *walking_left;
    Animation *walking_up_left;

    Animation *spell_animation_up;
    Animation *spell_animation_up_right;
    Animation *spell_animation_right;
    Animation *spell_animation_down_right;
    Animation *spell_animation_down;
    Animation *spell_animation_down_left;
    Animation *spell_animation_left;
    Animation *spell_animation_up_left;

};
//class shoot:: public Mage()
//{
    
//}

class MyLayer : public Layer{
  public:
    
    void AddTileImage(Image *image, Vector2D FrameSet ) {
      Sprite* chao;
      int i, j;
      
      Vector2D TamTela = Engine::reference()->video_manager()->video_size();
      
      for( i=0; i < TamTela.x / FrameSet.x; i++ )  //tosco
        for( j=0; j < TamTela.y / FrameSet.y; j++ ){
          chao = new Sprite();
          chao->Initialize( image );
          chao->image()->set_frame_size(FrameSet);
          chao->set_position(Vector2D( i*FrameSet.x, j*FrameSet.y ));
          this->AddSprite(chao);
        }
    }
};

class MageScene : public Scene {

public:
    enum dir {UP,UP_LEFT,LEFT,DOWN_LEFT,DOWN,DOWN_RIGHT,RIGHT,UP_RIGHT};
    MageScene() {
        tiro = new Projetil(Engine::reference()->video_manager()->LoadImage("skeleton4.png"), Vector2D(280, 210), Vector2D(74, 61));
        mage_ = new Mage(Engine::reference()->video_manager()->LoadImage("mage3.png"), Vector2D(280, 210), Vector2D(74, 61) );
        video_ = Engine::reference()->video_manager();
        input_ = Engine::reference()->input_manager();
//        main_layer_ = new Layer(Engine::reference()->video_manager()->LoadImage("skeleton4.png"), Vector2D(280, 210), Vector2D(74, 61));
        main_layer_ = new Layer(Vector2D(0, 0));

mage_->magia = tiro;
chao_layer_ = new MyLayer();
chao_layer_->AddTileImage(Engine::reference()->video_manager()->LoadImage("floor7.png"), Vector2D(42,42));

        main_layer_->AddSprite(tiro);
        main_layer_->AddSprite(mage_);
        this->AddLayer(chao_layer_);
//        main_layer_->AddSprite(tiro);
//        printf("foi\n");

        this->AddLayer(main_layer_);
    }

    double Angle( Vector2D v ) {
//      double param, result;
//      float schu = Vector2D::InnerProduct( u, v )/( u.length() * v.length() );       
//      param = cos( schu );
//      result = (double) acos( schu ) * (180.0 / 3.14159265);
//      printf( "u = ( %lf, %lf ), v = ( %lf, %lf )\n", u.x, u.y, v.x, v.y );
//      printf ( "< u, v > = %lf, lenght( u ) = %lf, lenght( v ) = %lf, schu: %f\n",
//               Vector2D::InnerProduct( u, v ), u.length(), v.length(), schu );      
//      printf ( "The arc cosine of %lf is %lf degrees.\n", param, result );            
//      double angle = atan(v.x/v.y) / (3.14159265/180);
//      printf ( "atan: %lf \n", angle );
//      if ( v.x < 0 )
//        angle += 180; 
//      if ( v.x >= 0 && v.y > 0 )
//        angle += 360;
//        else
          // adjusting for negative xy with mouse movement...
                      // think of the mouse moving to four quadrants, (1,1), (-1, 1), (-1, -1), (1, -1)
//      printf ( "adjust: %lf \n", angle );
//      angle = 180 - (atan(v.y/v.x)*atan(v.y/v.x))/(3.14159265*180);
//      printf( "new: %lf \n", angle );
            
      double angle;
      double mpi = 3.14159265;

      v.y = -v.y;      
//      printf( "v = ( %lf, %lf )\n", v.x, v.y );
      angle = acos( v.x / v.length( ) );
      if( v.y < 0 ) // corrige erros
       angle = 2*mpi - angle;      
//      printf( "acos(v.x/v.length()) = %lf \n", angle );
//      printf( "acos(v.x/v.length()) = %lf \n", angle / ( mpi / 180 ) );
//      printf( "asin(v.y/v.length()) = %lf \n", asin(v.y/v.length()) / (3.14159265/180) );
//      printf( "acos(v.x/v.length()) + asin(v.y/v.length()) = %lf \n", ( acos(v.x/v.length()) + asin(v.y/v.length()))/ (3.14159265/180) );
//      printf( "v.x/v.length() = %lf \n", v.x/v.length() );
//      printf( "v.y/v.length() = %lf \n", v.y/v.length() );
//      printf( "v.x/v.length() + v.y/v.length() = %lf \n", v.x/v.length() + v.y/v.length() );
//      printf( " = %lf \n",  );
      

//      if()
      decide(angle);
      return angle;
    }
    void Update(float delta_t) {
        Scene::Update(delta_t);
        video_->backbuffer()->Clear(0);
        Get_keys();

/* 9 9  10 9 11 9
   9 10 1010 1110
   9 11 1011 1111 */
   /* testes */
        Vector2D mage(10.0,10.0);
        Vector2D origem(0,-1);
        origem = mage;        
        Vector2D mouse(10.0, 9);
        Angle( Vector2D::Subtract( mouse, mage ) );
        mouse.x = 11; mouse.y = 9;
        Angle( Vector2D::Subtract( mouse, mage ) );
        mouse.x = 11; mouse.y = 10;
        Angle( Vector2D::Subtract( mouse, mage ) );
        mouse.x = 11; mouse.y = 11;
        Angle( Vector2D::Subtract( mouse, mage ) );
        mouse.x = 10; mouse.y = 11;
        Angle( Vector2D::Subtract( mouse, mage ) );
        mouse.x = 9; mouse.y = 11;
        Angle( Vector2D::Subtract( mouse, mage ) );
        mouse.x = 9; mouse.y = 10;
        Angle( Vector2D::Subtract( mouse, mage ) );
        mouse.x = 9; mouse.y = 9;
        Angle( Vector2D::Subtract( mouse, mage ) );
//        printf("END\n");
//        input_->Update(SDL_DEFAULT_REPEAT_INTERVAL);
//fprintf()
    }

  private:
    Mage *mage_;
    VideoManager *video_;
    Layer *main_layer_;
    InputManager *input_;

    
    MyLayer *chao_layer_;
    Projetil *tiro;
    int decide( float angle )
    {
      double mpi = 3.14159265;
      double h = mpi/4;
      double Sn  = mpi/8; // Sn = 0.5*h;
      double Sn1 = Sn + h;
      int i;
      for( i = 0; i < 8; i++ ){
//        printf( "Sn = %lf \n", Sn);
//        printf( "Sn1 = %lf \n", Sn1 );
//        printf( "%d\n", i );
        if( Sn < angle && angle <  Sn1 )        
          switch( i )
          {
            case 0 :
              return UP_RIGHT;
              break;
            case 1 :
              return UP;
              break;
            case 2 :
              return UP_LEFT;
              break;
            case 3 :
              return LEFT;
              break;
            case 4 :
              return DOWN_LEFT;
              break;
            case 5 :
              return DOWN;
              break;
            case 6 :
              return DOWN_RIGHT;
              break;
            case 7 :
              return RIGHT;
              break;
          }
        
        Sn = Sn + h;       
        Sn1 = Sn + h;
      }
      
//      if( h < angle || angle < 15*h )
        return RIGHT;
    }


//    SDL_Event keyevent;
/*    
    void Get_keys() {
//SDL_EnableKeyRepeat(0, 0);
//    Uint8 *keystate = SDL_GetKeyState(NULL);

    SDL_PollEvent(&keyevent);

        if(keyevent.key.keysym.sym == SDLK_ESCAPE)
            Engine::reference()->quit();
        if(keyevent.key.keysym.sym == SDLK_SPACE)
            mage_->Spell();

     if(keyevent.type == SDL_KEYUP){
//  else {
            mage_->is_walking_ = false;
        if( keyevent.key.keysym.sym == SDLK_UP){
            mage_->direction_ = UP;
          }
  //          mage_->Stand();
          }
        if(keyevent.type == SDL_KEYDOWN){
//    else{
            mage_->is_walking_ = true;
        if( keyevent.key.keysym.sym == SDLK_UP){
            mage_->direction_ = UP;
          }
//            mage_->Walk();
          }

//            mage_->Walk();
      
      }
      */
    
/*
    void Get_keys() {
while (SDL_PollEvent(&keyevent))   //Poll our SDL key event for any keystrokes.
{
      if(keyevent.key.keysym.sym == K_ESCAPE)
            Engine::reference()->quit();
  switch(keyevent.type){
    case SDL_KEYDOWN:
      switch(keyevent.key.keysym.sym){
        case SDLK_LEFT:
            mage_->direction_ = LEFT;
          break;
        case SDLK_RIGHT:
            mage_->direction_ = RIGHT;
          break;
        case SDLK_UP:
            mage_->direction_ = UP;
          break;
        case SDLK_DOWN:
            mage_->direction_ = DOWN;
          break;
        default:
          break;
       }
            mage_->Walk();
            break;
    case SDL_KEYUP:
      switch(keyevent.key.keysym.sym){
        case SDLK_LEFT:
            mage_->direction_ = LEFT;
          break;
        case SDLK_RIGHT:
            mage_->direction_ = RIGHT;
          break;
        case SDLK_UP:
            mage_->direction_ = UP;
          break;
        case SDLK_DOWN:
            mage_->direction_ = DOWN;
          break;
        default:
          break;
      }
            mage_->Stand();
            break;
  }
 }
}
*/
  
    void Get_keys() {
//      SDL_Event event;
//          tiro->set_position(mage_->position());
                  
//      while( SDL_PollEvent( &event ) ){
        if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)){
          Vector2D mouse = input_->GetMouseState();
          printf("Mouse Button 1(left) is pressed.\n");
          mage_->direction_ = decide(Angle(Vector2D::Subtract( mouse, mage_->position() ) ) );
          mage_->is_walking_ = false;
//          Mage tiro = new Mage();
//          Mage *tiro;
//          tiro->speed = 1;
//          tiro->direction_ = mage_->direction_;
//          tiro->is_walking_ = true;
//          tiro->Walk();
          mage_->Spell(mouse);
//          mage_->Spell();
          }
//        switch( event.type ) { 
//          case SDL_MOUSEMOTION:
//           printf("We got a motion event.\n");
//           printf("Current mouse position is: (%d, %d)\n", event.motion.x, event.motion.y);

 //          break;
 //         case SDL_MOUSEBUTTONDOWN:
 //           printf("Current mouse position button press is: (%d, %d)\n", event.motion.x, event.motion.y);
 //           mage_->direction_ = decide(Angle(Vector2D::Subtract( input_->GetMouseState(), mage_->position() ) ) );
 //           mage_->is_walking_ = false;
 //           mage_->Walk();
 //           break;
 //         case SDL_KEYDOWN:
 //         case SDL_KEYUP:
 //         case SDL_PRESSED:
            if(input_->KeyDown(K_ESCAPE))
              Engine::reference()->quit();
            else if(input_->KeyDown(K_SPACE))
//              mage_->Spell();
              mage_->Spell(input_->GetMouseState());
            else if(input_->KeyDown(K_w)){
              if(input_->KeyDown(K_d))
                mage_->direction_ = UP_RIGHT;
              else if(input_->KeyDown(K_a))
                mage_->direction_ = UP_LEFT;
              else
                mage_->direction_ = UP;
              mage_->is_walking_ = true;
              mage_->Walk();
            }
            else if(input_->KeyDown(K_s)){
              if(input_->KeyDown(K_d))
                mage_->direction_ = DOWN_RIGHT;
              else if(input_->KeyDown(K_a))
                mage_->direction_ = DOWN_LEFT;
              else
                mage_->direction_ = DOWN;
              mage_->is_walking_ = true;
              mage_->Walk();
//          return;
            }
            else if(input_->KeyDown(K_a)){
             mage_->direction_ = LEFT;
             mage_->is_walking_ = true;
             mage_->Walk();
            }
            else if(input_->KeyDown(K_d)){
            mage_->direction_ = RIGHT;
            mage_->is_walking_ = true;
            mage_->Walk();
            }
            else{
             mage_->is_walking_ = false;
             mage_->Walk();  
            }
//            break;
//          default:
//            printf("Unhandled Event!\n");
//            break;
//        }
//        }
//        }      
//
//        }
      }
                  
//            while( SDL_PollEvent(&event) ){
//if(event.type)

//        if( event.type == SDL_MOUSEBUTTONDOWN){
//            mage_->direction_ = decide(Angle(Vector2D::Subtract( input_->GetMouseState(), mage_->position() ) ) );
//            mage_->is_walking_ = false;
//            mage_->Walk();
//            break;
//}else
//{
//    }

/*    void Get_keys() {
    Uint8 *keystate = SDL_GetKeyState(NULL);
      SDL_PollEvent(&keyevent);
      
        if(keystate[K_ESCAPE])
            Engine::reference()->quit();
        if(keystate[K_SPACE])
            mage_->Spell();
if(keyevent.type == SDL_KEYDOWN ){
      mage_->is_walking_ = false;
if ( keystate[K_w] && keystate[K_d] ){
//   printf("Return Key Pressed.\n");
            mage_->direction_ = UP_RIGHT;
//            mage_->Walk();
   }


        if(input_->KeyDown(K_w)) {
            mage_->direction_ = UP;
//            mage_->Walk();
        }
        if(input_->KeyDown(K_d)) {
            mage_->direction_ = RIGHT;
//            mage_->Walk();
        }
        if(input_->KeyDown(K_s)) {
            mage_->direction_ = DOWN;
//            mage_->Walk();
        }
        if(input_->KeyDown(K_a)) {
            mage_->direction_ = LEFT;
//            mage_->Walk();
        }
  }
    if(keyevent.type == SDL_KEYUP ){
      mage_->is_walking_ = false;
//            mage_->Stand();
        if(keystate[K_UP] && keystate[K_RIGHT]){          
            mage_->direction_ = UP_RIGHT;
//            mage_->Stand();
        }else{
          if(keystate[K_UP]) {
            mage_->direction_ = UP;
//            mage_->Stand();
          }
          if(keystate[K_RIGHT]){          
            mage_->direction_ = RIGHT;
//            mage_->Stand();
          }          
        }
        if(keystate[K_DOWN] && keystate[K_RIGHT]){          
            mage_->direction_ = DOWN_RIGHT;
//            mage_->Stand();
        }else{
          if(keystate[K_DOWN]) {
            mage_->direction_ = DOWN;
//            mage_->Stand();
          }          
        }
        if(keystate[K_DOWN] && keystate[K_LEFT]) {
            mage_->direction_ = DOWN_LEFT;
//            mage_->Stand();
        }else{
          if(keystate[K_LEFT]) {
            mage_->direction_ = LEFT;
//            mage_->Stand();
          }
        }
        if(keystate[K_UP] && keystate[K_LEFT]) {
            mage_->direction_ = UP_LEFT;
//            mage_->Stand();
        }
      }

} */
};

int main(int argc, char* argv[]) {
    Engine * engine = Engine::reference();
    engine->Initialize("Mage", Vector2D(800, 600), false);

    MageScene * scene = new MageScene();
//printf("foi main");    
//    scene->input_ = engine->input_manager();

    engine->PushScene(scene);

    engine->Run();
    engine->Release();

    return 0;
}
