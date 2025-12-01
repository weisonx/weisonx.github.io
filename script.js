/* 
   Weisonx Portfolio Logic
   Style: Liquid Glass / Apple Design
*/

const GITHUB_USERNAME = "weisonx"; 
const MAX_REPOS = 6; 

// --- 1. 主题管理 (Theme Manager) ---
const themeToggleBtn = document.getElementById('theme-toggle');
const themeIcon = document.getElementById('theme-icon');
const htmlEl = document.documentElement;

function updateThemeIcon(theme) {
    if (theme === 'light') {
        themeIcon.className = 'fas fa-moon'; // 切到深色
    } else {
        themeIcon.className = 'fas fa-sun'; // 切到浅色
    }
}

function setTheme(theme) {
    htmlEl.setAttribute('data-theme', theme);
    localStorage.setItem('theme', theme);
    updateThemeIcon(theme);
}

// 初始化主题
const savedTheme = localStorage.getItem('theme');
const systemPrefersDark = window.matchMedia('(prefers-color-scheme: dark)').matches;

if (savedTheme) {
    setTheme(savedTheme);
} else {
    setTheme(systemPrefersDark ? 'dark' : 'light');
}

themeToggleBtn.addEventListener('click', () => {
    const currentTheme = htmlEl.getAttribute('data-theme');
    const newTheme = currentTheme === 'dark' ? 'light' : 'dark';
    setTheme(newTheme);
});

// --- 2. 打字机效果 (更加极简流畅) ---
const phrases = ["Developer.", "Designer.", "Creator.", "Engineer."]; // 带句号，更像 Apple 文案
let phraseIndex = 0;
let charIndex = 0;
let isDeleting = false;
const typeSpeed = 120;
const deleteSpeed = 60;
const pauseTime = 2500;

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
        const [userRes, reposRes] = await Promise.all([
            fetch(`https://api.github.com/users/${GITHUB_USERNAME}`),
            fetch(`https://api.github.com/users/${GITHUB_USERNAME}/repos?sort=pushed&per_page=100`)
        ]);

        if (!userRes.ok || !reposRes.ok) throw new Error('GitHub API Error');

        const user = await userRes.json();
        const repos = await reposRes.json();

        // 填充用户信息
        document.getElementById('avatar').src = user.avatar_url;
        document.getElementById('username').innerText = user.name || user.login;
        document.getElementById('bio').innerText = user.bio || "Crafting digital experiences.";
        
        // 填充统计 (嵌入 Hero 中)
        const statsContainer = document.getElementById('stats');
        statsContainer.innerHTML = `
            <div class="stat-item"><span class="stat-num">${user.public_repos}</span><span class="stat-label">Works</span></div>
            <div class="stat-item"><span class="stat-num">${user.followers}</span><span class="stat-label">Followers</span></div>
        `;

        // 过滤和排序仓库
        const topRepos = repos
            .filter(r => !r.fork)
            .sort((a, b) => b.stargazers_count - a.stargazers_count)
            .slice(0, MAX_REPOS);

        const repoContainer = document.getElementById('repos-container');
        repoContainer.innerHTML = ''; 

        topRepos.forEach(repo => {
            const card = document.createElement('a');
            card.href = repo.html_url;
            card.target = "_blank";
            card.className = 'card glass-card'; // 应用玻璃类
            
            // 更加柔和的语言颜色
            const langColors = {
                'JavaScript': '#f7df1e', 'Python': '#3572A5', 'Java': '#b07219', 
                'TypeScript': '#3178c6', 'HTML': '#e34c26', 'CSS': '#563d7c', 
                'Vue': '#41b883', 'C++': '#f34b7d'
            };
            const langColor = langColors[repo.language] || '#a0a0a0';

            card.innerHTML = `
                <div class="repo-name">
                    ${repo.name}
                </div>
                <p class="repo-desc">${repo.description ? repo.description.slice(0, 80) + (repo.description.length > 80 ? '...' : '') : "No description available."}</p>
                <div class="repo-meta">
                    <span style="display:flex;align-items:center"><span class="language-dot" style="background-color: ${langColor}; box-shadow: 0 0 8px ${langColor}"></span>${repo.language || 'Code'}</span>
                    <span>★ ${repo.stargazers_count}</span>
                </div>
            `;
            repoContainer.appendChild(card);
        });

    } catch (error) {
        console.error(error);
        document.getElementById('bio').innerText = "Unavailable to load data.";
    }
}

loadGitHubData();