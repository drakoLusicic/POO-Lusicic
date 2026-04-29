#ifndef VENTANA_H
#define VENTANA_H

#include "pantalla.h"
#include <QKeyEvent>
#include <QPixmap>

namespace Ui { class Ventana; }

class Ventana : public Pantalla
{
    Q_OBJECT

public:
    explicit Ventana(const QPixmap &fondo, const QString &profilePhotoPath, QWidget *parent = nullptr);
    ~Ventana() override;
    void inicializar() override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void ajustarImagenPrincipal();
    void cargarFotoPerfil();

    Ui::Ventana *ui;
    QPixmap mainImage;
    QString profilePhotoPath;
};

#endif // VENTANA_H
