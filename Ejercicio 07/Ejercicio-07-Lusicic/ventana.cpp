#include "ventana.h"
#include "ui_ventana.h"
#include <QApplication>

Ventana::Ventana(const QPixmap &fondo, const QString &profilePhotoPath, QWidget *parent)
    : Pantalla(parent), ui(new Ui::Ventana), mainImage(fondo), profilePhotoPath(profilePhotoPath)
{
    ui->setupUi(this);
}

Ventana::~Ventana()
{
    delete ui;
}

void Ventana::inicializar()
{
    setWindowTitle("Perfil Profesional Dark Tech");
    ui->titleLabel->setText("Currículum profesional");
    ui->subtitleLabel->setText("Especialista en interfaces orientadas a objetos y experiencia de usuario Dark Tech");

    cargarFotoPerfil();
    ui->photoLabel->setStyleSheet("background-color: #121820; border-radius: 18px; border: 2px solid #3e69d1; color: #c7d1ff; font-size: 12pt;");

    ui->descriptionLabel->setText(
        "<b>Sobre mí</b><br>Ingeniero en Informatica con pasión por la programación orientada a objetos, diseño elegante y soluciones robustas. "
        "Me especializo en aplicaciones de escritorio con interfaces modernas, integración de APIs y soporte para entornos desconectados.");

    ui->profileDetailsLabel->setText(
        "<b>Experiencia</b><br>• Desarrollo de aplicaciones Qt con arquitectura clara y componentes reutilizables.<br>"
        "• Integración de servicios REST y manejo avanzado de redes.<br>"
        "• Diseño de interfaces Dark Tech y dashboards de alto impacto.<br><br>"
        "<b>Herramientas</b><br>Qt, C++, QNetworkAccessManager, QSettings, JSON, UI/UX profesional.");

    ajustarImagenPrincipal();
    registrarEvento("Ventana principal inicializada con perfil profesional.");
}

void Ventana::resizeEvent(QResizeEvent *event)
{
    Pantalla::resizeEvent(event);
    ajustarImagenPrincipal();
}

void Ventana::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        registrarEvento("Escape presionado: cerrando ventana principal.");
        qApp->quit();
        return;
    }
    Pantalla::keyPressEvent(event);
}

void Ventana::cargarFotoPerfil()
{
    if (!profilePhotoPath.isEmpty() && QPixmap(profilePhotoPath).isNull() == false) {
        QPixmap foto(profilePhotoPath);
        ui->photoLabel->setPixmap(foto.scaled(160, 160, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        ui->photoLabel->setStyleSheet("border-radius: 18px; border: 2px solid #3e69d1;");
        return;
    }
    ui->photoLabel->setText("Perfil\nProfesional");
}

void Ventana::ajustarImagenPrincipal()
{
    if (!mainImage.isNull()) {
        const QPixmap scaled = mainImage.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        ui->backgroundLabel->setPixmap(scaled);
    }
}
