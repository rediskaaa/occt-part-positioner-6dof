#include "core/ModelEntity.hpp"

class OcctQOpenGLWidgetViewer;

class SceneManager {
public:
    explicit SceneManager(OcctQOpenGLWidgetViewer* viewer);
    void setShape();
    void setOnModelLoaded(std::function<void()> cb);
    void transformShape(double x, double y, double z, double angleX, double angleY, double angleZ);

private:
    OcctQOpenGLWidgetViewer* m_viewer;
    std::vector<ModelEntity> m_shapes;
    
};