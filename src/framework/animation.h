#ifndef HORUSEYE_FRAMEWORK_ANIMATION_H_
#define HORUSEYE_FRAMEWORK_ANIMATION_H_
#include <vector>
#include "observer.h"
using namespace std;
namespace framework {

class Animation {
  public:
	// Primeiro parametro define o fps da animacao.
	// Os parametros seguintes sao os indices dos frames da animacao.
	// Lista de parametro e' encerrada com um -1
	//
	//  A imagem e' dividida usando o frame_size, e o indice de um frame
	//  e' representado pela sua posicao na imagem fonte, em linhas, da
	//  esquerda para a direita e de cima para baixo.
    Animation(float fps, ...);
    ~Animation();

    void set_framelist(int frame1, ...);

    void set_fps(float fps) { fps_ = fps; }
    float fps() { return fps_; }
    int n_frames() { return n_frames_; }

    int get_current_frame() { return frames_[current_frame_]; }
    void Update(float delta_t);
    void AddObserver(Observer* observer);

  private:
    float fps_;
    int *frames_;
    int n_frames_;
    int current_frame_;
    float elapsed_time_;
    vector<Observer *> observers;
    void NotifyAllObservers();

    void CopyFrameList(int frame_list[], int n_frames);

};

}
#endif


