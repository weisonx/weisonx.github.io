const GITHUB_USERNAME = "weisonx";
const MAX_REPOS = 6;
// 移除 MAX_FILES_PREVIEW，改为全局遍历

// --- 1. 主题切换 ---
const htmlEl = document.documentElement;
const themeIcon = document.getElementById('theme-icon');

function setTheme(theme) {
    htmlEl.setAttribute('data-theme', theme);
    localStorage.setItem('theme', theme);
    themeIcon.className = theme === 'light' ? 'fas fa-moon' : 'fas fa-sun';
}

const initialTheme = localStorage.getItem('theme') || (window.matchMedia('(prefers-color-scheme: dark)').matches ? 'dark' : 'light');
setTheme(initialTheme);

document.getElementById('theme-toggle').addEventListener('click', () => {
    setTheme(htmlEl.getAttribute('data-theme') === 'dark' ? 'light' : 'dark');
});

// --- 2. 打字机 ---
const phrases = ["架构师", "工程师", "开发者", "Slow is smooth, smooth is fast."];
let pIdx = 0, cIdx = 0, isDeleting = false;

function type() {
    const text = phrases[pIdx];
    const el = document.getElementById('typing-text');
    el.textContent = text.substring(0, isDeleting ? cIdx-- : cIdx++);
    
    if (!isDeleting && cIdx > text.length) {
        isDeleting = true;
        setTimeout(type, 2000);
    } else if (isDeleting && cIdx < 0) {
        isDeleting = false;
        pIdx = (pIdx + 1) % phrases.length;
        cIdx = 0;
        setTimeout(type, 500);
    } else {
        setTimeout(type, isDeleting ? 50 : 100);
    }
}

// --- 3. GitHub 数据 ---
async function fetchGitHub() {
    try {
        const [uRes, rRes] = await Promise.all([
            fetch(`https://api.github.com/users/${GITHUB_USERNAME}`),
            fetch(`https://api.github.com/users/${GITHUB_USERNAME}/repos?sort=updated&per_page=30`)
        ]);

        const user = await uRes.json();
        const repos = await rRes.json();

        // 填充 Hero
        document.getElementById('avatar').src = user.avatar_url;
        document.getElementById('username').innerText = user.name || user.login;
        document.getElementById('bio').innerText = user.bio || "Digital Craftsman";
        document.getElementById('stats').innerHTML = `
            <div class="stat-item"><span class="stat-num">${user.public_repos}</span><span class="stat-label">Repos</span></div>
            <div class="stat-item"><span class="stat-num">${user.followers}</span><span class="stat-label">Followers</span></div>
        `;

        // 填充项目 (按星标排序)
        const topRepos = repos.filter(r => !r.fork).sort((a,b) => b.stargazers_count - a.stargazers_count).slice(0, MAX_REPOS);
        document.getElementById('repos-container').innerHTML = topRepos.map(r => `
            <a href="${r.html_url}" target="_blank" class="card glass-card">
                <div class="repo-name"><i class="fab fa-github"></i> ${r.name}</div>
                <p class="repo-desc">${r.description || "No description provided."}</p>
                <div class="repo-meta">
                    <span><i class="fas fa-code-branch"></i> ${r.language || 'Mixed'}</span>
                    <span>★ ${r.stargazers_count}</span>
                </div>
            </a>
        `).join('');

        // 初始化 Home Show (展示星数最多的仓库)
        if (topRepos.length > 0) initHomeShow(topRepos[0]);

    } catch (e) {
        console.error(e);
        document.getElementById('bio').innerText = "API rate limit reached. Please check back later.";
    }
}

// --- 4. Home Show 逻辑 (遍历所有文件) ---
let filesCache = [];

async function initHomeShow(repo) {
    const btn = document.getElementById('generate-cards');
    const container = document.getElementById('file-cards-container');

    async function loadFiles() {
        btn.disabled = true;
        const icon = btn.querySelector('i');
        icon.classList.add('fa-spin');
        
        try {
            // 获取递归全量树
            const treeRes = await fetch(`https://api.github.com/repos/${GITHUB_USERNAME}/${repo.name}/git/trees/${repo.default_branch}?recursive=1`);
            const treeData = await treeRes.json();
            
            // 过滤：仅保留文件(blob)，排除隐藏文件、依赖包和大型二进制目录
            const allFiles = treeData.tree.filter(i => 
                i.type === 'blob' && 
                !i.path.startsWith('.') && 
                !i.path.includes('node_modules/') &&
                !i.path.includes('dist/') &&
                !i.path.includes('vendor/')
            );
            
            // 为了防止请求过多导致浏览器崩溃，我们采取以下策略：
            // 1. 展示所有文件卡片
            // 2. 仅对前 20 个文本文件尝试抓取预览内容
            filesCache = await Promise.all(allFiles.map(async (f, index) => {
                let content = "";
                const isPreviewable = /\.(js|ts|py|html|css|md|txt|json|yml|c|cpp|go|rs)$/.test(f.path);
                
                // 限制：仅前20个预览文件，且大小小于50KB
                if (index < 20 && f.size < 50000 && isPreviewable) {
                    try {
                        const cRes = await fetch(`https://raw.githubusercontent.com/${GITHUB_USERNAME}/${repo.name}/${repo.default_branch}/${f.path}`);
                        if (cRes.ok) content = await cRes.text();
                    } catch(e) { content = ""; }
                }

                return { ...f, content, repoName: repo.name };
            }));

            renderFiles(filesCache);
        } catch (err) {
            console.error(err);
            container.innerHTML = `<p style="grid-column: 1/-1; text-align: center; opacity: 0.5;">无法加载文件树 (可能达到 API 限制)</p>`;
        } finally {
            btn.disabled = false;
            icon.classList.remove('fa-spin');
        }
    }

    btn.onclick = loadFiles;
    loadFiles();
}

function renderFiles(files) {
    const container = document.getElementById('file-cards-container');
    if (files.length === 0) {
        container.innerHTML = `<p style="grid-column: 1/-1; text-align: center; opacity: 0.5;">No files found.</p>`;
        return;
    }

    container.innerHTML = files.map(f => {
        const parts = f.path.split('.');
        const ext = parts.length > 1 ? parts.pop().toLowerCase() : '';
        const isCode = ["js", "ts", "py", "html", "css", "md", "json", "yml", "c", "cpp", "go", "rs", "sql"].includes(ext);
        const fileName = f.path.split('/').pop();
        
        return `
            <div class="file-card glass-card" onclick="window.open('https://github.com/${GITHUB_USERNAME}/${f.repoName}/blob/main/${f.path}', '_blank')">
                <div class="file-card-size">${(f.size/1024).toFixed(1)} KB</div>
                <div class="file-card-icon ${isCode ? 'code' : 'binary'}">
                    <i class="fas ${isCode ? 'fa-code' : (ext === 'md' ? 'fa-book' : 'fa-file-alt')}"></i>
                </div>
                <div class="file-card-name">${fileName}</div>
                <div class="file-card-path">${f.path}</div>
                <div class="file-card-content">${
                    f.content 
                    ? f.content.substring(0, 120).replace(/</g, '&lt;').replace(/>/g, '&gt;') 
                    : '<span style="opacity:0.4; font-style:italic;">No preview available</span>'
                }</div>
            </div>
        `;
    }).join('');
}

// 搜索过滤
document.getElementById('file-search').addEventListener('input', (e) => {
    const term = e.target.value.toLowerCase();
    const filtered = filesCache.filter(f => f.path.toLowerCase().includes(term));
    renderFiles(filtered);
});

document.addEventListener('DOMContentLoaded', () => {
    type();
    fetchGitHub();
});