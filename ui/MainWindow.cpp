#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "infrastructure/SceneManager.hpp"
#include "infrastructure/StepImporter.hpp"

#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    m_sceneManager = std::make_unique<SceneManager>(ui->openGLWidget);
    connect(ui->xSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::spinBoxChanged);
    connect(ui->ySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::spinBoxChanged);
    connect(ui->zSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::spinBoxChanged);
    connect(ui->xAngleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::spinBoxChanged);
    connect(ui->yAngleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::spinBoxChanged);
    connect(ui->zAngleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::spinBoxChanged);
    connect(ui->actionImport_STEP, &QAction::triggered,
            this, &MainWindow::onLoadButtonClicked);
}

void MainWindow::spinBoxChanged() {
    m_sceneManager->transformShape(ui->xSpinBox->value(), ui->ySpinBox->value(), ui->zSpinBox->value(), 
        ui->xAngleSpinBox->value(), ui->yAngleSpinBox->value(), ui->zAngleSpinBox->value());
}

void MainWindow::onLoadButtonClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open STEP File", "", "STEP Files (*.step *.stp)");

    if (!filePath.isEmpty()) {
        const auto path = std::filesystem::path(
            filePath.toUtf8().constData()
        );
        auto shape = StepImporter::importFile(path);
        if (shape.has_value()) {
            m_sceneManager->addShape(*shape);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}