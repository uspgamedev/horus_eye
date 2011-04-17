#ifndef HORUSEYE_FRAMEWORK_FONT_H_
#define HORUSEYE_FRAMEWORK_FONT_H_

#include <string>
#include "vector2D.h"

namespace framework {

using std::string;
class Image;
class Font {
  public:
	enum IdentType {
		LEFT,
		CENTER,
		RIGHT
	};
	Font(Image ** letters, int fontsize, char ident, bool fancy);
	~Font();
	int id() { return id_; }
	IdentType ident() { return ident_; }
	Vector2D GetLetterSize(unsigned char letter);
	bool IsFancy() { return fancy_;}

  private:
	int id_;
	int size_;
	Image ** letters_;
	IdentType ident_;
	bool fancy_;
};


}  // namespace framework

#endif
