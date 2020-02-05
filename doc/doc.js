let ctx = document.getElementById("lifecycle").getContext("2d");
let c = {x: 0, y: 100};      // cursor
ctx.font = "14px Arial";
const headlen = 10; // length of head in pixels
const state_radius = 20;
const states = [
    {name: "start", x: 0, y: 100},
    {name: "A", x: 200, y: 100},
    {name: "C", x: 200, y: 300},
    {name: "B", x: 400, y: 100},
    {name: "D", x: 600, y: 100},
    {name: "E", x: 600, y: 300},
    {name: "F", x: 600, y: 500},
    {name: "G'", x: 800, y: 300},
    {name: "G''", x: 1000, y: 300},
    {name: "H", x: 800, y: 100},
    {name: "end", x: 1000, y: 100}
];

const transitions = [
    ["start", "A", 0, false],
    ["A", "B", 0.5, false],
    ["B", "C", 0, false],
    ["B", "D", 0, false],
    ["C", "B", 0.5, false],
    ["D", "E", 0, false],
    ["E", "B", 0.5, false],
    ["E", "D", 0.5, false],
    ["E", "F", 0, false],
    ["E", "G'", 0, false],
    ["F", "E", 0.5, false],
    ["G'", "E", 0.5, false],
    ["G''", "H", 0, false],
    ["D", "H", 0, false],
    ["H", "B", 0.5, true],
    ["H", "G''", 0.5, true],
    ["H", "end", 0, false]
];

for (let i = 0; i < states.length; i++) {
    if (states[i].name !== "start" && states[i].name !== "end") {
        state(ctx, states[i].name, states[i].x, states[i].y);
    }
}

for (let i = 0; i < transitions.length; i++) {
    let j = 0, k = 0;
    while (j < states.length) {
        if (states[j].name === transitions[i][0]) {
            break;
        }
        j++;
    }
    while (k < states.length) {
        if (states[k].name === transitions[i][1]) {
            break;
        }
        k++;
    }
    canvas_arrow(ctx, states[j].x, states[j].y, states[k].x, states[k].y, transitions[i][2], transitions[i][3]);
}


function state(ctx, name, x, y) {
    ctx.beginPath();
    ctx.arc(x, y, state_radius, 0, 2 * Math.PI);
    ctx.font = "16px Arial";
    ctx.fillText(name, x - 6, y + 6);
    ctx.stroke();
}

function canvas_arrow(ctx, fromx, fromy, tox, toy, curve, opposite) {
    let dx = tox - fromx;
    let dy = toy - fromy;
    if (opposite) {
        dx = fromx - tox;
        dy = fromy - toy;
    }
    const angle = Math.atan2(dy, dx);
    const dist = Math.sqrt(Math.pow(dx, 2) + Math.pow(dy, 2));
    let center = {x: Math.abs(fromx + tox) / 2, y: Math.abs(fromy + toy) / 2};
    let radius = dist / 2;
    center.x -= radius * (1 - curve) * Math.sin(angle) * 3.3;
    center.y += radius * (1 - curve) * Math.cos(angle) * 3.3;
    ctx.beginPath();
    if (curve === 0) {
        fromx += state_radius * Math.cos(angle);
        fromy += state_radius * Math.sin(angle);
        tox -= state_radius * Math.cos(angle);
        toy -= state_radius * Math.sin(angle);
        ctx.moveTo(fromx, fromy);
        ctx.lineTo(tox, toy);
        arrowhead(ctx, headlen, angle, tox, toy);
    } else {
        let startAngle = angle + Math.PI * curve * 2.725 - radius * 0.0003;
        let endAngle = angle - Math.PI * curve * 2.725 - Math.PI + radius * 0.0003;
        ctx.arc(center.x, center.y, radius / curve, startAngle, endAngle);
        if (!opposite){
            tox = center.x + radius / curve * Math.cos(endAngle);
            toy = center.y + radius / curve * Math.sin(endAngle);
            arrowhead(ctx, headlen, angle + Math.PI / 6, tox, toy);
        } else {
            fromx = center.x + radius / curve * Math.cos(startAngle);
            fromy = center.y + radius / curve * Math.sin(startAngle);
            arrowhead(ctx, headlen, angle - Math.PI / 6 + Math.PI, fromx, fromy);
        }
    }
    ctx.stroke();
}

function arrowhead(context, headlen, angle, tox, toy) {
    context.moveTo(tox, toy);
    context.lineTo(tox - headlen * Math.cos(angle - Math.PI / 6), toy - headlen * Math.sin(angle - Math.PI / 6));
    context.moveTo(tox, toy);
    context.lineTo(tox - headlen * Math.cos(angle + Math.PI / 6), toy - headlen * Math.sin(angle + Math.PI / 6));
}