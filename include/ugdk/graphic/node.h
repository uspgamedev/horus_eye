#ifndef UGDK_GRAPHIC_NODE_H_
#define UGDK_GRAPHIC_NODE_H_

#include <vector>
#include <ugdk/graphic.h>
#include <ugdk/graphic/modifier.h>

namespace ugdk {
namespace graphic {

/// 
class Node {
  public:
    typedef std::vector<Node*> NodeSet;

    Node(Drawable* drawable = NULL, Modifier* modifier = NULL) 
        :   modifier_(modifier), 
            drawable_(drawable),
            light_(NULL),
            active_(true),
            zindex_(0.0),
            parent_(NULL) {}

    ~Node();

    /// Pushes the modifier to the VideoManager, renders 
    void Render(double dt);
    void RenderLight() const;

    void set_drawable(Drawable* drawable) { drawable_ = drawable; }
    void set_light(Light* light)          {    light_ =    light; }
    void set_active(const bool active)    {   active_ =   active; }
    void set_zindex(const double zindex);
    
          Modifier* modifier() { return (modifier_ != NULL) ? modifier_ : (modifier_ = new Modifier); }
    const Modifier* modifier() const { return modifier_; }
          Drawable* drawable()       { return drawable_; }
    const Drawable* drawable() const { return drawable_; }
          Light* light()             { return    light_; }
    const Light* light()       const { return    light_; }
          bool   active()      const { return   active_; }
          double  zindex()      const { return   zindex_; }

    void AddChild(Node *child) {
        childs_.push_back(child);
        child->parent_ = this;
    }
    void RemoveChild(Node *child);


    static bool CompareByZIndex(const Node *a, const Node *b);
    void SortChildren();

  private:
    Modifier* modifier_;
    Drawable* drawable_;
    Light* light_;
    bool active_;
    double zindex_;

    NodeSet childs_;
    Node* parent_;
    bool must_sort_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_NODE_H_
