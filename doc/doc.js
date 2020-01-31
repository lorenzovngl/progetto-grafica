let ctx = document.getElementById("lifecycle").getContext("2d");
let c = {x: 0, y: 100};      // cursor
ctx.font = "14px Arial";
const headlen = 10; // length of head in pixels
const state_radius = 20;
const states = [
    {name: "start", x: 0, y: 200},
    {name: "A", x: 200, y: 200},
    {name: "B", x: 400, y: 200},
    {name: "C", x: 600, y: 200},
    {name: "D", x: 800, y: 200},
    {name: "end", x: 1000, y: 200},
];
const transitions = [
    [0, 1, 0, 0, 0, 0],
    [0, 0, 1, 0, 0, 0],
    [0, 0, 0, 1, 0, 0],
    [0, 1, 0, 0, 1, 0],
    [0, 0, 0, 0, 0, 1],
    [0, 0, 0, 0, 0, 0]
];

for (let i = 0; i < states.length; i++) {
    if (states[i].name !== "start" && states[i].name !== "end"){
        state(ctx, states[i].name, states[i].x, states[i].y);
    }
}

for (let i = 0; i < transitions.length; i++){
    for (let j = 0; j < transitions[i].length; j++){
        console.log(transitions[i][j] === 1);
        if (transitions[i][j] === 1){
            canvas_arrow(ctx, states[i].x, states[i].y, states[j].x, states[j].y, true);
        }
    }
}

/*ctx.beginPath();
canvas_arrow(ctx, c.x, c.y + 80, c.x + 160, c.y + 80);
ctx.stroke();
ctx.fillText("Lancio", c.x + 60, c.y + 50);
ctx.fillText("dell'applicazione", c.x + 30, c.y + 70);
state(c.x + 180, c.y + 80);

c.x = 200;
ctx.beginPath();
canvas_arrow(ctx, c.x, c.y + 80, c.x + 160, c.y + 80);
ctx.stroke();
ctx.fillText("Inserimento del", c.x + 10, c.y + 50);
ctx.fillText("nome utente", c.x + 10, c.y + 70);
state(c.x + 180, c.y + 80);

ctx.beginPath();
ctx.arc(c.x + 145, c.y + 180, 100, Math.PI, 1.4 * Math.PI);
arrowhead(ctx, headlen, 1.95*Math.PI, c.x + 131, c.y + 85);
ctx.stroke();
ctx.beginPath();
ctx.arc(c.x + 30, c.y + 95, 100, 2* Math.PI, 0.4 * Math.PI);
ctx.stroke();
ctx.fillText("Scelta del", c.x, c.y + 230);
ctx.fillText("numero di boe", c.x, c.y + 250);
state(c.x + 45, c.y + 195);

ctx.beginPath();
canvas_arrow(ctx, c.x + 200, c.y + 80, c.x + 260, c.y + 80);
ctx.stroke();
ctx.fillText("Inizio della", c.x + 150, c.y + 50);
ctx.fillText("sessione di gioco", c.x + 140, c.y + 70);
state(c.x + 280, c.y + 80);

ctx.beginPath();
ctx.arc(c.x + 340, c.y + 150, 100, 0.8 * Math.PI, 1.2 * Math.PI);
ctx.stroke();
ctx.beginPath();
ctx.arc(c.x + 200, c.y + 150, 100, 1.8 * Math.PI, 0.2 * Math.PI);
ctx.stroke();
ctx.fillText("Gioco in", c.x + 180, c.y + 240);
ctx.fillText("pausa", c.x + 190, c.y + 260);
state(c.x + 270, c.y + 220);

ctx.beginPath();
ctx.arc(c.x + 310, c.y + 50, 180, 0.6 * Math.PI, 0.92 * Math.PI);
ctx.stroke();

ctx.beginPath();
ctx.arc(c.x + 340, c.y + 290, 100, 0.8 * Math.PI, 1.2 * Math.PI);
ctx.stroke();
ctx.beginPath();
ctx.arc(c.x + 200, c.y + 290, 100, 1.8 * Math.PI, 0.2 * Math.PI);
ctx.stroke();
ctx.fillText("Visualizzazione", c.x + 130, c.y + 350);
ctx.fillText("dei comandi", c.x + 140, c.y + 370);
state(c.x + 270, c.y + 360);

ctx.beginPath();
ctx.arc(c.x + 340, c.y + 290, 100, 1.3 * Math.PI, 1.7 * Math.PI);
ctx.stroke();
ctx.beginPath();
ctx.arc(c.x + 340, c.y + 150, 100, 0.3 * Math.PI, 0.7 * Math.PI);
ctx.stroke();
ctx.fillText("Visualizzazione", c.x + 405, c.y + 180);
ctx.fillText("della leaderboard", c.x + 400, c.y + 200);
state(c.x + 410, c.y + 220);

ctx.beginPath();
canvas_arrow(ctx, c.x + 290, c.y + 80, c.x + 390, c.y + 80);
ctx.stroke();
ctx.fillText("Fine della", c.x + 300, c.y + 50);
ctx.fillText("sessione di gioco", c.x + 280, c.y + 70);
state(c.x + 410, c.y + 80);

ctx.beginPath();
ctx.arc(c.x + 270, c.y + 70, 140, 1.03 * Math.PI, 1.97 * Math.PI);
arrowhead(ctx, headlen, Math.PI/2, c.x + 131, c.y + 60);
ctx.stroke();

ctx.beginPath();
canvas_arrow(ctx, c.x + 430, c.y + 80, c.x + 540, c.y + 80);
ctx.stroke();
ctx.fillText("Chiusura", c.x + 450, c.y + 50);
ctx.fillText("dell'applicazione", c.x + 430, c.y + 70);*/

function state(ctx, name, x, y) {
    ctx.beginPath();
    ctx.arc(x, y, state_radius, 0, 2 * Math.PI);
    ctx.font = "16px Arial";
    ctx.fillText(name, x - 6, y + 6);
    ctx.stroke();
}

function canvas_arrow(ctx, fromx, fromy, tox, toy, curved) {
    const dx = tox - fromx;
    const dy = toy - fromy;
    const angle = Math.atan2(dy, dx);
    const dist = Math.sqrt(Math.pow(dx, 2) + Math.pow(dy, 2));
    let curving = 0.5;
    let center = {x: Math.abs(fromx+tox)/2, y: Math.abs(fromy+toy)/2};
    let radius = dist/2;
    center.x -= radius * (1 - curving) * Math.sin(angle) * 3.3;
    center.y += radius * (1 - curving) * Math.cos(angle) * 3.3;
    ctx.beginPath();
    if (!curved){
        fromx += state_radius * Math.cos(angle);
        fromy += state_radius * Math.sin(angle);
        tox -= state_radius * Math.cos(angle);
        toy -= state_radius * Math.sin(angle);
        ctx.moveTo(fromx, fromy);
        ctx.lineTo(tox, toy);
        arrowhead(ctx, headlen, angle, tox, toy);
    } else {
        let startAngle = angle + Math.PI * curving * 2.705;
        let endAngle = angle - Math.PI * curving * 2.705 - Math.PI;
        ctx.arc(center.x, center.y, radius/curving, startAngle, endAngle);
        tox = center.x + radius/curving * Math.cos(endAngle);
        toy = center.y + radius/curving * Math.sin(endAngle);
        arrowhead(ctx, headlen, angle + Math.PI/6, tox, toy);
    }
    ctx.stroke();
}

function arrowhead(context, headlen, angle, tox, toy) {
    context.moveTo(tox, toy);
    context.lineTo(tox - headlen * Math.cos(angle - Math.PI / 6), toy - headlen * Math.sin(angle - Math.PI / 6));
    context.moveTo(tox, toy);
    context.lineTo(tox - headlen * Math.cos(angle + Math.PI / 6), toy - headlen * Math.sin(angle + Math.PI / 6));
}