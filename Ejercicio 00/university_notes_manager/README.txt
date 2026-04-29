GESTOR DE NOTAS UNIVERSITARIAS - XAMPP

1) Copiá la carpeta university_notes_manager dentro de htdocs.
   Ejemplo:
   C:\xampp\htdocs\university_notes_manager

2) Abrí phpMyAdmin.

3) Importá el archivo:
   sql/gestor_notas.sql

4) Verificá que tu MySQL tenga:
   usuario: root
   contraseña: vacía

5) Si usás otra configuración, editá:
   config/db.php

6) Abrí en el navegador:
   http://localhost/university_notes_manager/

FUNCIONALIDADES:
- Alta de materias
- Alta de notas
- Modificación de notas
- Eliminación de notas
- Cálculo de promedio
- Estado automático:
  * 1 a 3 = Desaprobado
  * 4 a 7.99 = Regular
  * 8 a 10 = Promocionado
- Barra de progreso por color:
  * rojo, amarillo, verde

SUPUESTOS TOMADOS:
- El estado se calcula con promedio simple de todas las evaluaciones cargadas.
- Cada materia tiene un profesor principal.
- El cuatrimestre se guarda como texto simple.
