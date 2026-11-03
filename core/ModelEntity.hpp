#include "ShapeModel.hpp"
#include "TransformController.hpp"

class AIS_Shape;

struct ModelEntity {
    uint64_t id;
    TopoDS_Shape shape;
    Handle(AIS_Shape) aisShape;
    gp_Trsf trsf;
};