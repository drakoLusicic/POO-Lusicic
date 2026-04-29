let currentSubject = null;

function calculateAverage(grades) {
    if (!grades || grades.length === 0) return 0;
    const total = grades.reduce((acc, item) => acc + Number(item.nota || 0), 0);
    return Number((total / grades.length).toFixed(2));
}

function calculateStatus(avg) {
    if (avg >= 8) return 'Promocionado';
    if (avg >= 4) return 'Regular';
    if (avg >= 1) return 'Desaprobado';
    return 'Sin notas';
}

function formatAverage(avg) {
    return Number(avg).toFixed(2);
}

function openCreateSubjectModal() {
    document.getElementById('createSubjectModal').classList.remove('hidden');
}

function closeCreateSubjectModal() {
    document.getElementById('createSubjectModal').classList.add('hidden');
}

function closeSubjectModal() {
    document.getElementById('subjectModal').classList.add('hidden');
    currentSubject = null;
}

function openSubjectModal(subject) {
    currentSubject = subject;
    const modal = document.getElementById('subjectModal');
    modal.classList.remove('hidden');

    document.getElementById('modalSubjectName').textContent = subject.nombre;
    document.getElementById('modalSubjectMeta').textContent = `${subject.cuatrimestre} · ${subject.anio}`;
    document.getElementById('modalProfessor').textContent = subject.profesor;

    const average = calculateAverage(subject.notas || []);
    const status = calculateStatus(average);

    document.getElementById('modalAverage').textContent = formatAverage(average);
    document.getElementById('modalStatus').textContent = status;

    document.getElementById('addGradeSubjectId').value = subject.id;
    document.getElementById('editGradeSubjectId').value = subject.id;
    document.getElementById('deleteGradeSubjectId').value = subject.id;

    renderGrades(subject.notas || []);
    populateEditAndDelete(subject.notas || []);
}

function renderGrades(grades) {
    const container = document.getElementById('gradesContainer');
    if (!grades.length) {
        container.innerHTML = `
            <div class="rounded-2xl border border-dashed border-slate-300 p-5 text-slate-500 bg-white">
                Esta materia todavía no tiene notas cargadas.
            </div>
        `;
        return;
    }

    container.innerHTML = grades.map(item => `
        <div class="rounded-2xl border border-slate-200 p-4 bg-white flex flex-col md:flex-row md:items-center md:justify-between gap-3">
            <div>
                <p class="font-semibold text-lg">${escapeHtml(item.tipo_evaluacion)}</p>
                <p class="text-slate-500 text-sm">Fecha: ${escapeHtml(item.fecha_nota || '-')}</p>
                <p class="text-slate-500 text-sm">${escapeHtml(item.observaciones || 'Sin observaciones')}</p>
            </div>
            <div class="px-4 py-2 rounded-2xl bg-slate-100 text-slate-800 font-bold text-lg min-w-[90px] text-center">
                ${Number(item.nota).toFixed(2)}
            </div>
        </div>
    `).join('');
}

function populateEditAndDelete(grades) {
    const editSelect = document.getElementById('editGradeSelect');
    const deleteSelect = document.getElementById('deleteGradeSelect');

    if (!grades.length) {
        editSelect.innerHTML = '<option value="">No hay notas</option>';
        deleteSelect.innerHTML = '<option value="">No hay notas</option>';
        document.getElementById('editTipo').value = '';
        document.getElementById('editNota').value = '';
        document.getElementById('editFecha').value = '';
        document.getElementById('editObservaciones').value = '';
        return;
    }

    const options = grades.map(item => `
        <option value="${item.id}">${escapeHtml(item.tipo_evaluacion)} - ${Number(item.nota).toFixed(2)} (${escapeHtml(item.fecha_nota || '-')})</option>
    `).join('');

    editSelect.innerHTML = options;
    deleteSelect.innerHTML = options;

    fillEditFormFromSelection(grades[0].id);
    editSelect.onchange = function() {
        fillEditFormFromSelection(Number(this.value));
    };
}

function fillEditFormFromSelection(selectedId) {
    if (!currentSubject || !currentSubject.notas) return;
    const grade = currentSubject.notas.find(item => Number(item.id) === Number(selectedId));
    if (!grade) return;

    document.getElementById('editTipo').value = grade.tipo_evaluacion || '';
    document.getElementById('editNota').value = grade.nota || '';
    document.getElementById('editFecha').value = grade.fecha_nota || '';
    document.getElementById('editObservaciones').value = grade.observaciones || '';
}

function escapeHtml(text) {
    return String(text)
        .replace(/&/g, '&amp;')
        .replace(/</g, '&lt;')
        .replace(/>/g, '&gt;')
        .replace(/"/g, '&quot;')
        .replace(/'/g, '&#039;');
}

window.addEventListener('keydown', (e) => {
    if (e.key === 'Escape') {
        closeSubjectModal();
        closeCreateSubjectModal();
    }
});
