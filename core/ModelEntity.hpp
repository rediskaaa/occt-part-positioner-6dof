#include "ShapeModel.hpp"
#include "TransformController.hpp"

class AIS_Shape;

struct ModelEntity {
    TopoDS_Shape shape;
    Handle(AIS_Shape) aisShape;
    gp_Trsf trsf;
};