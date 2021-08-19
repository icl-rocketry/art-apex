window.createChart = (id, xMin, xMax, yMin, yMax, colour) => {
    let ctx = document.getElementById(id).getContext('2d');
    let data = [];
    let myChart = new Chart(ctx, {
        type: 'line',
        data: {
            datasets: [{
                label: "X acceleration",
                data: data,
                radius: 0,
                borderColor: colour,
                tension: 0.01,
                borderWidth: 1
            }]
        },
        options: {
            scales: {
                xAxis: {
                    max: xMax,
                    min: xMin,
                    grid: {
                        drawBorder: false,
                        display: false
                    },
                    type: "linear"
                },
                yAxis: {
                    max: yMax,
                    min: yMin,
                    grid: {
                        drawBorder: false,
                        display: false
                    }
                },
            },
            plugins: {
                legend: false
            }
        }
    });

    eval(`
    window.update_${id} = (x, y) => {
        data.push({
            x: x,
            y: y
        });
        myChart.update("none");
    }
    `);
}
