/* 
   配置项：请在此处修改为你的 GitHub 用户名 
*/
const GITHUB_USERNAME = "weisonx"; // <--- 修改这里！例如 "torvalds"
const MAX_REPOS = 6; // 展示仓库数量

// --- 1. Canvas 粒子星空背景 ---
const canvas = document.getElementById('bg-canvas');
const ctx = canvas.getContext('2d');

let particles = [];
const particleCount = 80; // 粒子数量
const connectionDistance = 150; // 连线距离
const mouseDistance = 200; // 鼠标吸引距离

let w, h;

function resize() {
    w = canvas.width = window.innerWidth;
    h = canvas.height = window.innerHeight;
}

window.addEventListener('resize', resize);
resize();

class Particle {
    constructor() {
        this.x = Math.random() * w;
        this.y = Math.random() * h;
        this.vx = (Math.random() - 0.5) * 0.5; // 速度
        this.vy = (Math.random() - 0.5) * 0.5;
        this.size = Math.random() * 2 + 1;
    }

    update() {
        this.x += this.vx;
        this.y += this.vy;

        // 边界反弹
        if (this.x < 0 || this.x > w) this.vx *= -1;
        if (this.y < 0 || this.y > h) this.vy *= -1;

        // 鼠标吸引逻辑
        let dx = mouse.x - this.x;
        let dy = mouse.y - this.y;
        let distance = Math.sqrt(dx * dx + dy * dy);
        if (distance < mouseDistance) {
            const forceDirectionX = dx / distance;
            const forceDirectionY = dy / distance;
            const force = (mouseDistance - distance) / mouseDistance;
            const directionX = forceDirectionX * force * 0.6;
            const directionY = forceDirectionY * force * 0.6;
            this.x += directionX;
            this.y += directionY;
        }
    }

    draw() {
        ctx.fillStyle = 'rgba(6, 182, 212, 0.6)';
        ctx.beginPath();
        ctx.arc(this.x, this.y, this.size, 0, Math.PI * 2);
        ctx.fill();
    }
}

// 初始化粒子
for (let i = 0; i < particleCount; i++) {
    particles.push(new Particle());
}

let mouse = { x: null, y: null };
window.addEventListener('mousemove', (e) => {
    mouse.x = e.x;
    mouse.y = e.y;
});

// 动画循环
function animate() {
    ctx.clearRect(0, 0, w, h);
    
    particles.forEach(p => {
        p.update();
        p.draw();
    });

    // 绘制连线
    for (let a = 0; a < particles.length; a++) {
        for (let b = a; b < particles.length; b++) {
            let dx = particles[a].x - particles[b].x;
            let dy = particles[a].y - particles[b].y;
            let distance = Math.sqrt(dx * dx + dy * dy);

            if (distance < connectionDistance) {
                let opacityValue = 1 - (distance / connectionDistance);
                ctx.strokeStyle = `rgba(6, 182, 212, ${opacityValue * 0.2})`;
                ctx.lineWidth = 1;
                ctx.beginPath();
                ctx.moveTo(particles[a].x, particles[a].y);
                ctx.lineTo(particles[b].x, particles[b].y);
                ctx.stroke();
            }
        }
    }
    requestAnimationFrame(animate);
}
animate();


// --- 2. 打字机效果 ---
const phrases = ["Developer & Engineer", "Full Stack Developer", "Tech Explorer", "Cpper"];
let phraseIndex = 0;
let charIndex = 0;
let isDeleting = false;
const typeSpeed = 100;
const deleteSpeed = 50;
const pauseTime = 2000;

function typeEffect() {
    const currentPhrase = phrases[phraseIndex];
    const typingElement = document.getElementById('typing-text');
    
    if (isDeleting) {
        typingElement.textContent = currentPhrase.substring(0, charIndex - 1);
        charIndex--;
    } else {
        typingElement.textContent = currentPhrase.substring(0, charIndex + 1);
        charIndex++;
    }

    if (!isDeleting && charIndex === currentPhrase.length) {
        isDeleting = true;
        setTimeout(typeEffect, pauseTime);
    } else if (isDeleting && charIndex === 0) {
        isDeleting = false;
        phraseIndex = (phraseIndex + 1) % phrases.length;
        setTimeout(typeEffect, 500);
    } else {
        setTimeout(typeEffect, isDeleting ? deleteSpeed : typeSpeed);
    }
}
document.addEventListener('DOMContentLoaded', typeEffect);


// --- 3. GitHub API 数据加载 ---
async function loadGitHubData() {
    try {
        // 并行获取用户信息和仓库信息
        const [userRes, reposRes] = await Promise.all([
            fetch(`https://api.github.com/users/${GITHUB_USERNAME}`),
            fetch(`https://api.github.com/users/${GITHUB_USERNAME}/repos?sort=pushed&per_page=100`)
        ]);

        if (!userRes.ok || !reposRes.ok) throw new Error('GitHub API Error');

        const user = await userRes.json();
        const repos = await reposRes.json();

        // 更新用户信息
        document.getElementById('avatar').src = user.avatar_url;
        document.getElementById('username').innerText = user.name || user.login;
        document.getElementById('bio').innerText = user.bio || "Writing code and building things.";
        
        // 更新统计卡片
        const statsContainer = document.getElementById('stats');
        statsContainer.innerHTML = `
            <div class="stat-card"><span class="stat-num">${user.public_repos}</span><span class="stat-label">Repos</span></div>
            <div class="stat-card"><span class="stat-num">${user.followers}</span><span class="stat-label">Followers</span></div>
            <div class="stat-card"><span class="stat-num">${user.following}</span><span class="stat-label">Following</span></div>
        `;

        // 筛选优质仓库（按 Star 排序，排除 Fork，或者你可以自定义逻辑）
        const topRepos = repos
            .filter(r => !r.fork) // 仅展示原创仓库
            .sort((a, b) => b.stargazers_count - a.stargazers_count)
            .slice(0, MAX_REPOS);

        // 渲染仓库卡片
        const repoContainer = document.getElementById('repos-container');
        repoContainer.innerHTML = ''; // 清除骨架屏

        topRepos.forEach(repo => {
            const card = document.createElement('a');
            card.href = repo.html_url;
            card.target = "_blank";
            card.className = 'card';
            
            // 语言颜色映射（可扩展）
            const langColors = {
                'JavaScript': '#f1e05a', 'Python': '#3572A5', 'Java': '#b07219', 
                'TypeScript': '#2b7489', 'HTML': '#e34c26', 'CSS': '#563d7c', 
                'Vue': '#41b883', 'C++': '#f34b7d'
            };
            const langColor = langColors[repo.language] || '#8b9bb4';

            card.innerHTML = `
                <div class="repo-name">
                    <i class="far fa-folder-open"></i> ${repo.name}
                </div>
                <p class="repo-desc">${repo.description || "No description available."}</p>
                <div class="repo-meta">
                    <span><span class="language-dot" style="background-color: ${langColor}"></span>${repo.language || 'Code'}</span>
                    <span><i class="far fa-star"></i> ${repo.stargazers_count}</span>
                    <span><i class="fas fa-code-branch"></i> ${repo.forks_count}</span>
                </div>
            `;
            repoContainer.appendChild(card);
        });

    } catch (error) {
        console.error(error);
        document.getElementById('bio').innerHTML = `<span style="color:#ef4444">System Malfunction: Failed to fetch data.</span>`;
    }
}

loadGitHubData();