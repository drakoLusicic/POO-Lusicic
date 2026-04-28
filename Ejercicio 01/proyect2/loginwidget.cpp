#include "loginwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QFont>

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent) {
    authManager = new AuthManager();
    setupUI();

    setWindowTitle("Login - Planificador de Trabajos Prácticos");
    resize(480, 520);
    setStyleSheet(
        "QWidget { background-color: #F0F4FF; font-family: 'Segoe UI', Arial; }"
    );
}

void LoginWidget::setupUI() {
    // Layout exterior centrado
    QVBoxLayout *outerLayout = new QVBoxLayout();
    outerLayout->setAlignment(Qt::AlignCenter);
    outerLayout->setContentsMargins(0, 0, 0, 0);

    // Card central
    QWidget *card = new QWidget();
    card->setFixedWidth(400);
    card->setStyleSheet(
        "QWidget {"
        "  background-color: #FFFFFF;"
        "  border-radius: 16px;"
        "  border: 1px solid #E2E8F0;"
        "}"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(card);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    // Ícono / badge superior
    QLabel *badgeLabel = new QLabel("TP");
    badgeLabel->setFixedSize(56, 56);
    badgeLabel->setAlignment(Qt::AlignCenter);
    badgeLabel->setStyleSheet(
        "background-color: #6366F1;"
        "color: white;"
        "border-radius: 28px;"
        "font-size: 20px;"
        "font-weight: bold;"
        "border: none;"
    );

    QHBoxLayout *badgeRow = new QHBoxLayout();
    badgeRow->addStretch();
    badgeRow->addWidget(badgeLabel);
    badgeRow->addStretch();
    mainLayout->addLayout(badgeRow);

    // Título
    titleLabel = new QLabel("Iniciar Sesión");
    QFont titleFont("Segoe UI", 20, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #1E293B; border: none; background: transparent;");
    mainLayout->addWidget(titleLabel);

    QLabel *subtitleLabel = new QLabel("Planificador de Trabajos Prácticos");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("color: #64748B; font-size: 12px; border: none; background: transparent;");
    mainLayout->addWidget(subtitleLabel);

    mainLayout->addSpacing(8);

    // Usuario
    QLabel *userLabel = new QLabel("Usuario");
    userLabel->setStyleSheet("color: #374151; font-weight: 600; font-size: 13px; border: none; background: transparent;");
    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("Ingrese su usuario");
    usernameEdit->setMinimumHeight(44);
    usernameEdit->setStyleSheet(
        "QLineEdit {"
        "  border: 1.5px solid #D1D5DB;"
        "  border-radius: 8px;"
        "  padding: 0 12px;"
        "  font-size: 14px;"
        "  color: #1E293B;"
        "  background-color: #F9FAFB;"
        "}"
        "QLineEdit:focus {"
        "  border: 1.5px solid #6366F1;"
        "  background-color: #FFFFFF;"
        "}"
    );
    mainLayout->addWidget(userLabel);
    mainLayout->addWidget(usernameEdit);

    // Contraseña
    QLabel *passLabel = new QLabel("Contraseña");
    passLabel->setStyleSheet("color: #374151; font-weight: 600; font-size: 13px; border: none; background: transparent;");
    passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("Ingrese su contraseña");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setMinimumHeight(44);
    passwordEdit->setStyleSheet(
        "QLineEdit {"
        "  border: 1.5px solid #D1D5DB;"
        "  border-radius: 8px;"
        "  padding: 0 12px;"
        "  font-size: 14px;"
        "  color: #1E293B;"
        "  background-color: #F9FAFB;"
        "}"
        "QLineEdit:focus {"
        "  border: 1.5px solid #6366F1;"
        "  background-color: #FFFFFF;"
        "}"
    );
    mainLayout->addWidget(passLabel);
    mainLayout->addWidget(passwordEdit);

    // Error
    errorLabel = new QLabel("");
    errorLabel->setStyleSheet(
        "color: #EF4444; font-size: 12px; font-weight: 600;"
        "border: none; background: transparent;"
    );
    errorLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(errorLabel);

    // Botón principal
    loginButton = new QPushButton("Iniciar Sesión");
    loginButton->setMinimumHeight(46);
    loginButton->setCursor(Qt::PointingHandCursor);
    loginButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #6366F1;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 10px;"
        "  font-size: 15px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #4F46E5; }"
        "QPushButton:pressed { background-color: #4338CA; }"
    );
    mainLayout->addWidget(loginButton);

    // Nota de usuarios
    QLabel *noteLabel = new QLabel("Prueba: admin / admin  |  estudiante1 / password123");
    noteLabel->setAlignment(Qt::AlignCenter);
    noteLabel->setStyleSheet(
        "color: #94A3B8; font-size: 10px; border: none; background: transparent;"
    );
    mainLayout->addWidget(noteLabel);

    connect(loginButton, &QPushButton::clicked, this, &LoginWidget::onLoginClicked);
    connect(passwordEdit, &QLineEdit::returnPressed, this, &LoginWidget::onPasswordEnterPressed);

    outerLayout->addWidget(card);
    setLayout(outerLayout);
}

void LoginWidget::onLoginClicked() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        errorLabel->setText("Por favor ingrese usuario y contraseña");
        return;
    }

    if (authManager->authenticate(username, password)) {
        authManager->saveSession(username);
        usernameEdit->clear();
        passwordEdit->clear();
        errorLabel->clear();
        emit loginSuccess(username);
    } else {
        errorLabel->setText("Usuario o contraseña incorrectos");
        passwordEdit->clear();
    }
}

void LoginWidget::onPasswordEnterPressed() {
    onLoginClicked();
}