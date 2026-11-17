(function () {
    "use strict"; // Ativa o modo restrito do JS

    // Lista com os nomes dos dias da semana (de Segunda a Domingo)
    const dayNames = [
        "Segunda",
        "Terca",
        "Quarta",
        "Quinta",
        "Sexta",
        "Sabado",
        "Domingo"
    ];

    // Lista com os nomes dos meses em português
    const monthNames = [
        "janeiro",
        "fevereiro",
        "marco",
        "abril",
        "maio",
        "junho",
        "julho",
        "agosto",
        "setembro",
        "outubro",
        "novembro",
        "dezembro"
    ];

    // Lista de horários disponíveis no dia (intervalos de 1 hora, das 08:00 às 17:00)
    const timeSlots = [
        "08:00",
        "09:00",
        "10:00",
        "11:00",
        "12:00",
        "13:00",
        "14:00",
        "15:00",
        "16:00",
        "17:00"
    ];

    // Dados de exemplo (simulando reservas passadas, atuais e futuras usando um offset de semanas)
    const bookingSeedByWeekOffset = {
        0: [ // Semana atual (offset 0)
            { day: 0, time: "08:00", room: "Sala X"},
            { day: 1, time: "08:00", room: "Sala X"},
            { day: 3, time: "08:00", room: "Sala 201"},
            { day: 5, time: "08:00", room: "Sala 105"},
            { day: 1, time: "10:00", room: "Lab. 02"},
            { day: 2, time: "10:00", room: "Sala 104"},
            { day: 4, time: "10:00", room: "Sala 104"},
            { day: 5, time: "10:00", room: "Sala 105"}
        ],
        1: [ // Próxima semana (offset 1)
            { day: 0, time: "09:00", room: "Sala 203"},
            { day: 2, time: "11:00", room: "Lab. 03"},
            { day: 4, time: "14:00", room: "Sala 110"},
            { day: 6, time: "10:00", room: "Auditorio"}
        ],
        "-1": [ // Semana anterior (offset -1)
            { day: 1, time: "08:00", room: "Sala 106"},
            { day: 3, time: "13:00", room: "Sala 202"},
            { day: 4, time: "15:00", room: "Lab. 01"}
        ]
    };

    // Seleciona os elementos principais da página HTML
    const calendarGrid = document.getElementById("calendar-grid");
    const weekRangeEl = document.getElementById("week-range");
    const weekNumberEl = document.getElementById("week-number");
    const totalBookingsEl = document.getElementById("total-bookings");
    const totalHoursEl = document.getElementById("total-hours");
    const prevWeekButton = document.getElementById("prev-week");
    const nextWeekButton = document.getElementById("next-week");

    const today = stripTime(new Date());
    let currentWeekOffset = 0;

    // Adiciona evento de clique no botão de voltar semana
    prevWeekButton.addEventListener("click", function () {
        currentWeekOffset -= 1;
        renderWeek();
    });

    // Adiciona evento de clique no botão de avançar semana
    nextWeekButton.addEventListener("click", function () {
        currentWeekOffset += 1;
        renderWeek();
    });

    renderWeek();

    // Função principal que coordena a montagem da semana na tela
    function renderWeek() {
        const weekStart = getWeekStart(addDays(today, currentWeekOffset * 7));
        
        // Cria um array com os 7 dias consecutivos usando um loop for tradicional
        const weekDays = [];
        for (let i = 0; i < 7; i += 1) {
            weekDays.push(addDays(weekStart, i));
        }

        const bookings = bookingSeedByWeekOffset[currentWeekOffset] || [];
        const bookingMap = buildBookingMap(bookings);

        updateToolbar(weekDays, bookings.length);
        buildCalendarGrid(weekDays, bookingMap);
    }

    // Função para atualizar as informações do cabeçalho
    function updateToolbar(weekDays, bookingCount) {
        const start = weekDays[0];
        const end = weekDays[6];

        weekRangeEl.textContent =
            String(start.getDate()).padStart(2, "0") +
            " - " +
            String(end.getDate()).padStart(2, "0") +
            " " +
            monthNames[end.getMonth()] +
            ", " +
            end.getFullYear();

        weekNumberEl.textContent = "SEMANA " + getIsoWeekNumber(start);
        totalBookingsEl.textContent = bookingCount + (bookingCount === 1 ? " reserva" : " reservas");
        totalHoursEl.textContent = bookingCount + (bookingCount === 1 ? " hora" : " horas");
    }

    // Função que constrói visualmente toda a grade do calendário
    function buildCalendarGrid(weekDays, bookingMap) {
        calendarGrid.innerHTML = "";

        const corner = document.createElement("div");
        corner.className = "calendar-corner";
        corner.innerHTML = "<span>HORARIO</span><small>Blocos de 1h</small>";
        calendarGrid.appendChild(corner);

        // Substituição do forEach por um loop for clássico
        for (let i = 0; i < weekDays.length; i += 1) {
            const date = weekDays[i];
            const heading = document.createElement("div");
            const isToday = isSameDate(date, today);
            heading.className = isToday ? "day-heading today" : "day-heading";

            let headingContent =
                "<span>" + dayNames[i] + "</span>" +
                "<small>" + formatDayMonth(date) + "</small>";

            if (isToday) {
                headingContent += "<small class=\"today-label\">HOJE</small>";
            }

            heading.innerHTML = headingContent;
            calendarGrid.appendChild(heading);
        }

        // Substituição do forEach de horários por um loop for clássico
        for (let t = 0; t < timeSlots.length; t += 1) {
            const time = timeSlots[t];
            const timeLabel = document.createElement("div");
            timeLabel.className = "time-label";
            timeLabel.innerHTML =
                "<strong>" + time + "</strong>" +
                "<small>as " + addOneHour(time) + "</small>";
            calendarGrid.appendChild(timeLabel);

            for (let dayIndex = 0; dayIndex < 7; dayIndex += 1) {
                const key = dayIndex + "_" + time;
                const booking = bookingMap[key];

                const cell = document.createElement("div");

                if (booking) {
                    cell.className = "calendar-cell has-booking";
                    cell.innerHTML =
                        "<div class=\"booking-title\"><i></i>" + booking.room + "</div>" +
                        "<div class=\"booking-location\"></div>" +
                        "<small class=\"booking-status\">CONFIRMADA</small>";
                } else {
                    cell.className = "calendar-cell";
                    cell.innerHTML =
                        "<span class=\"empty-label\">Sem reserva</span>" +
                        "<a href=\"../reserva/reserva.html\"><span class=\"btn-reserve\">+ Reservar</span></a>";
                }

                calendarGrid.appendChild(cell);
            }
        }
    }

    // Transforma uma lista comum de reservas em um objeto indexado
    function buildBookingMap(bookings) {
        const map = {};
        for (let i = 0; i < bookings.length; i += 1) {
            const booking = bookings[i];
            map[booking.day + "_" + booking.time] = booking;
        }
        return map;
    }

    function getWeekStart(date) {
        const day = date.getDay();
        const diff = day === 0 ? -6 : 1 - day;
        return addDays(date, diff);
    }

    function addDays(date, days) {
        const next = new Date(date);
        next.setDate(next.getDate() + days);
        return stripTime(next);
    }

    function stripTime(date) {
        return new Date(date.getFullYear(), date.getMonth(), date.getDate());
    }

    function formatDayMonth(date) {
        const day = String(date.getDate()).padStart(2, "0");
        const month = String(date.getMonth() + 1).padStart(2, "0");
        return day + "/" + month;
    }

    function addOneHour(time) {
        const hour = Number(time.slice(0, 2));
        const nextHour = String(hour + 1).padStart(2, "0");
        return nextHour + ":00";
    }

    function getIsoWeekNumber(date) {
        const target = new Date(Date.UTC(date.getFullYear(), date.getMonth(), date.getDate()));
        const dayNumber = target.getUTCDay() || 7;
        target.setUTCDate(target.getUTCDate() + 4 - dayNumber);
        const yearStart = new Date(Date.UTC(target.getUTCFullYear(), 0, 1));
        return Math.ceil((((target - yearStart) / 86400000) + 1) / 7);
    }

    function isSameDate(first, second) {
        return (
            first.getFullYear() === second.getFullYear() &&
            first.getMonth() === second.getMonth() &&
            first.getDate() === second.getDate()
        );
    }
})();