#include "TopoDS_Shape.hxx"

class ShapeModel {
public:
    explicit ShapeModel(TopoDS_Shape shape) : m_shape(shape) {}
    void setShape(TopoDS_Shape shape);

private:
    TopoDS_Shape m_shape{};
};