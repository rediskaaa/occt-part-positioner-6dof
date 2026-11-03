#include "SceneManager.hpp"

#include "BRepPrimAPI_MakeBox.hxx"
#include "AIS_Shape.hxx"
#include "TopoDS_Shape.hxx"
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>
#include <gp_Ax1.hxx>
#include <gp_Dir.hxx>
#include "gp_Quaternion.hxx"

#include "ui/OcctQOpenGLWidgetViewer.hpp"
#include "AIS_InteractiveContext.hxx"


// Вспомогательная функция для перемещения формы в пространстве
TopoDS_Shape MoveShape(const TopoDS_Shape& aShape, double dx, double dy, double dz) {
    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(dx, dy, dz));
    return BRepBuilderAPI_Transform(aShape, aTrsf).Shape();
}

TopoDS_Shape CreateGeometricCat() {
    // 1. ТУЛОВИЩЕ (основа)
    TopoDS_Shape aBody = BRepPrimAPI_MakeBox(40.0, 70.0, 40.0).Shape();
    // Центрируем туловище относительно оси X
    aBody = MoveShape(aBody, -20.0, -35.0, 0.0);

    // 2. ГОЛОВА
    TopoDS_Shape aHead = BRepPrimAPI_MakeBox(35.0, 35.0, 35.0).Shape();
    // Смещаем голову наверх и вперед (к передней части туловища)
    aHead = MoveShape(aHead, -17.5, 20.0, 35.0);

    // 3. УШИ (левое и правое)
    TopoDS_Shape aEarLeft = BRepPrimAPI_MakeBox(10.0, 8.0, 15.0).Shape();
    aEarLeft = MoveShape(aEarLeft, -12.0, 25.0, 70.0);
    TopoDS_Shape aEarRight = BRepPrimAPI_MakeBox(10.0, 8.0, 15.0).Shape();
    aEarRight = MoveShape(aEarRight, 2.0, 25.0, 70.0);

    // 4. ЛАПЫ (4 штуки)
    double legSize = 12.0;
    double legHeight = 20.0;
    TopoDS_Shape aLegTemplate = BRepPrimAPI_MakeBox(legSize, legSize, legHeight).Shape();

    TopoDS_Shape aLeg1 = MoveShape(aLegTemplate, -22.0, -30.0, -20.0); // Передняя левая
    TopoDS_Shape aLeg2 = MoveShape(aLegTemplate, 10.0, -30.0, -20.0);  // Передняя правая
    TopoDS_Shape aLeg3 = MoveShape(aLegTemplate, -22.0, 20.0, -20.0);   // Задняя левая
    TopoDS_Shape aLeg4 = MoveShape(aLegTemplate, 10.0, 20.0, -20.0);    // Задняя правая

    // 5. ХВОСТ (вытянутый цилиндр или брусок)
    TopoDS_Shape aTail = BRepPrimAPI_MakeCylinder(4.0, 50.0).Shape();
    // Немного наклоним хвост (можно повернуть через gp_Trsf с поворотом вокруг оси X/Y)
    gp_Trsf aTailTrsf;
    aTailTrsf.SetRotation(gp_Ax1(gp_Pnt(0, -35, 0), gp_Dir(1, 0, 0)), 0.3); // наклон назад
    aTail = BRepBuilderAPI_Transform(aTail, aTailTrsf).Shape();
    aTail = MoveShape(aTail, 0.0, -35.0, 10.0);

    // --- ОБЪЕДИНЕНИЕ ВСЕХ ЧАСТЕЙ В ОДИН SHAPE ---
    TopoDS_Shape aCat = aBody;
    
    // Последовательно склеиваем элементы через BRepAlgoAPI_Fuse
    auto FuseShapes = [](const TopoDS_Shape& s1, const TopoDS_Shape& s2) {
        BRepAlgoAPI_Fuse mkFuse(s1, s2);
        mkFuse.Build();
        return mkFuse.Shape();
    };

    aCat = FuseShapes(aCat, aHead);
    aCat = FuseShapes(aCat, aEarLeft);
    aCat = FuseShapes(aCat, aEarRight);
    aCat = FuseShapes(aCat, aLeg1);
    aCat = FuseShapes(aCat, aLeg2);
    aCat = FuseShapes(aCat, aLeg3);
    aCat = FuseShapes(aCat, aLeg4);
    aCat = FuseShapes(aCat, aTail);

    return aCat;
}

SceneManager::SceneManager(OcctQOpenGLWidgetViewer* viewer) : m_viewer(viewer) {
    addShape(CreateGeometricCat());
}

uint64_t SceneManager::addShape(const TopoDS_Shape& shape) {
    ModelEntity model;
    model.id = m_nextId++;
    model.shape = shape;
    model.aisShape = new AIS_Shape(model.shape);
    m_viewer->Context()->Display(model.aisShape, AIS_Shaded, 0, Standard_True);

    m_shapes.emplace(model.id, std::move(model));  

    return model.id;
}

void SceneManager::transformShape(double x, double y, double z, double angleX, double angleY, double angleZ) {
    auto it = m_shapes.find(m_nextId - 1);
    auto currModel = ((it != m_shapes.end()) ? &it->second : NULL);
    if (!currModel) return;

    gp_Trsf move, rotate, result;
    move.SetTranslation(gp_Vec(x, y, z));

    gp_Trsf rot1, rot2, rot3;
    rot1.SetRotation(gp_Ax1(gp_Pnt(0,0,0), gp_Dir(1,0,0)), angleX * M_PI / 180); 
    rot2.SetRotation(gp_Ax1(gp_Pnt(0,0,0), gp_Dir(0,1,0)), angleY * M_PI / 180); 
    rot3.SetRotation(gp_Ax1(gp_Pnt(0,0,0), gp_Dir(0,0,1)), angleZ * M_PI / 180); 

    rotate = rot1;
    rotate.Multiply(rot2);
    rotate.Multiply(rot3);

    result = move * rotate;   
    currModel->trsf = result;

    m_viewer->Context()->SetLocation(currModel->aisShape, currModel->trsf);
    m_viewer->Context()->Redisplay(currModel->aisShape, Standard_True);
}
