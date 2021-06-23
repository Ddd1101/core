#include "CStatisticsWidget.h"
#include "CMapModel.h"

#include <QResizeEvent>
#include <QStandardItemModel>

CStatisticsWidget::CStatisticsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    this->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    ui->setupUi(this);
}

CStatisticsWidget::~CStatisticsWidget()
{
    delete ui;
}

void CStatisticsWidget::SetMainWindow(QWidget *pMainWidget)
{
    this->pMainWindow = pMainWidget;

    if(NULL != pMainWidget)
        pMainWidget->setEnabled(false);
}

void CStatisticsWidget::SetStatistics(QMap<QString, unsigned int>* mStaticstics)
{
    CMapModel *pMapModel = new CMapModel;
    pMapModel->SetMap(mStaticstics);
    ui->StatisticsView->setModel(pMapModel);
}

void CStatisticsWidget::resizeEvent(QResizeEvent *pResizeEvent)
{
    unsigned int unWorkingPartHeight    = pResizeEvent->size().height();
    unsigned int unWorkingPartWidth     = pResizeEvent->size().width();

    ui->verticalLayout->setGeometry(QRect(0, 0, unWorkingPartWidth, unWorkingPartHeight));
}

void CStatisticsWidget::on_StatisticsExit_clicked()
{
    if (NULL != pMainWindow)
        pMainWindow->setEnabled(true);

    this->close();
}
