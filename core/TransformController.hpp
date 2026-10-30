#pragma once
#include "gp_Trsf.hxx"

class TransformController {
public:
    explicit TransformController() {}
    void setTransform();
private:
    gp_Trsf m_trsf;
};