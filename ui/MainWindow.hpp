#pragma once
#include <QMainWindow>
#include <memory>

class SceneManager;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();
private:
	void spinBoxChanged();
	void onLoadButtonClicked();
	Ui::MainWindow* ui;
	std::unique_ptr<SceneManager> m_sceneManager;
};