#include "core/ModelEntity.hpp"

class OcctQOpenGLWidgetViewer;

class SceneManager {
public:
    explicit SceneManager(OcctQOpenGLWidgetViewer* viewer);
    void setShape();
    void setOnModelLoaded(std::function<void()> cb);
    void transformShape(double x, double y, double z, double angleX, double angleY, double angleZ);
    uint64_t addShape(const TopoDS_Shape& shape);

private:
    OcctQOpenGLWidgetViewer* m_viewer;
    std::unordered_map<uint64_t, ModelEntity> m_shapes;
    uint64_t m_nextId{1};
};