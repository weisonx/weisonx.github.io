const GITHUB_USERNAME = "weisonx";
const MAX_REPOS = 6;

/* ===============================
   1. 主题切换
================================ */
const htmlEl = document.documentElement;
const themeIcon = document.getElementById('theme-icon');

function setTheme(theme) {
    htmlEl.setAttribute('data-theme', theme);
    localStorage.setItem('theme', theme);
    themeIcon.className = theme === 'light' ? 'fas fa-moon' : 'fas fa-sun';
}

const initialTheme =
    localStorage.getItem('theme') ||
    (window.matchMedia('(prefers-color-scheme: dark)').matches ? 'dark' : 'light');

setTheme(initialTheme);

document.getElementById('theme-toggle').addEventListener('click', () => {
    setTheme(htmlEl.getAttribute('data-theme') === 'dark' ? 'light' : 'dark');
});

/* ===============================
   2. 打字机
================================ */
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

/* ===============================
   3. GitHub 数据
================================ */
async function fetchGitHub() {
    try {
        const [uRes, rRes] = await Promise.all([
            fetch(`https://api.github.com/users/${GITHUB_USERNAME}`),
            fetch(`https://api.github.com/users/${GITHUB_USERNAME}/repos?sort=updated&per_page=30`)
        ]);

        const user = await uRes.json();
        const repos = await rRes.json();

        document.getElementById('avatar').src = user.avatar_url;
        document.getElementById('username').innerText = user.name || user.login;
        document.getElementById('bio').innerText = user.bio || "Digital Craftsman";
        document.getElementById('stats').innerHTML = `
            <div class="stat-item"><span class="stat-num">${user.public_repos}</span><span class="stat-label">Repos</span></div>
            <div class="stat-item"><span class="stat-num">${user.followers}</span><span class="stat-label">Followers</span></div>
        `;

        const topRepos = repos
            .filter(r => !r.fork)
            .sort((a, b) => b.stargazers_count - a.stargazers_count)
            .slice(0, MAX_REPOS);

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

        if (topRepos.length > 0) initHomeShow(topRepos[0]);

    } catch (e) {
        console.error(e);
        document.getElementById('bio').innerText =
            "API rate limit reached. Please check back later.";
    }
}

/* ===============================
   4. Home Show（懒加载核心）
================================ */
const FILES_PER_BATCH = 30;
let filesCache = [];
let renderIndex = 0;
let batchObserver = null;
let previewObserver = null;

async function initHomeShow(repo) {
    const btn = document.getElementById('generate-cards');
    const container = document.getElementById('file-cards-container');

    async function loadFiles() {
        btn.disabled = true;
        btn.querySelector('i').classList.add('fa-spin');
        container.innerHTML = "";
        filesCache = [];
        renderIndex = 0;

        try {
            const treeRes = await fetch(
                `https://api.github.com/repos/${GITHUB_USERNAME}/${repo.name}/git/trees/${repo.default_branch}?recursive=1`
            );
            const treeData = await treeRes.json();

            filesCache = treeData.tree
                .filter(i =>
                    i.type === 'blob' &&
                    !i.path.startsWith('.') &&
                    !i.path.includes('node_modules/') &&
                    !i.path.includes('dist/') &&
                    !i.path.includes('vendor/')
                )
                .map(f => ({
                    ...f,
                    repoName: repo.name,
                    previewLoaded: false
                }));

            renderNextBatch();
            setupBatchObserver();
            setupPreviewObserver();

        } catch (err) {
            console.error(err);
            container.innerHTML =
                `<p style="grid-column:1/-1;text-align:center;opacity:.5">无法加载文件</p>`;
        } finally {
            btn.disabled = false;
            btn.querySelector('i').classList.remove('fa-spin');
        }
    }

    btn.onclick = loadFiles;
    loadFiles();
}

/* ===============================
   5. 分批渲染卡片
================================ */
function renderNextBatch() {
    const container = document.getElementById('file-cards-container');
    const slice = filesCache.slice(renderIndex, renderIndex + FILES_PER_BATCH);

    slice.forEach(f => {
        container.insertAdjacentHTML('beforeend', createFileCardHTML(f));
    });

    renderIndex += FILES_PER_BATCH;
}

/* ===============================
   6. 卡片 HTML（UI 未改）
================================ */
function createFileCardHTML(f) {
    const ext = f.path.split('.').pop().toLowerCase();
    const isCode = ["js","ts","py","html","css","md","json","yml","c","cpp","go","rs","sql"].includes(ext);
    const fileName = f.path.split('/').pop();

    return `
        <div class="file-card glass-card"
             data-path="${f.path}"
             onclick="window.open('https://github.com/${GITHUB_USERNAME}/${f.repoName}/blob/main/${f.path}', '_blank')">
            <div class="file-card-size">${(f.size/1024).toFixed(1)} KB</div>
            <div class="file-card-icon ${isCode ? 'code' : 'binary'}">
                <i class="fas ${isCode ? 'fa-code' : 'fa-file-alt'}"></i>
            </div>
            <div class="file-card-name">${fileName}</div>
            <div class="file-card-path">${f.path}</div>
            <div class="file-card-content">
                <span style="opacity:0.4;font-style:italic;">Loading preview...</span>
            </div>
        </div>
    `;
}

/* ===============================
   7. 批次懒加载（滚动）
================================ */
function setupBatchObserver() {
    if (batchObserver) batchObserver.disconnect();

    batchObserver = new IntersectionObserver(entries => {
        if (entries[0].isIntersecting && renderIndex < filesCache.length) {
            renderNextBatch();
        }
    }, { rootMargin: '300px' });

    batchObserver.observe(document.querySelector('.glass-footer'));
}

/* ===============================
   8. Preview 懒加载
================================ */
function setupPreviewObserver() {
    if (previewObserver) previewObserver.disconnect();

    previewObserver = new IntersectionObserver(entries => {
        entries.forEach(e => {
            if (!e.isIntersecting) return;

            const card = e.target;
            const path = card.dataset.path;
            const file = filesCache.find(f => f.path === path);
            if (!file || file.previewLoaded) return;

            loadPreview(card, file);
            previewObserver.unobserve(card);
        });
    }, { rootMargin: '200px' });

    document.querySelectorAll('.file-card').forEach(c => previewObserver.observe(c));
}

async function loadPreview(card, file) {
    const isPreviewable = /\.(js|ts|py|html|css|md|txt|json|yml|c|cpp|go|rs)$/.test(file.path);
    if (!isPreviewable || file.size > 50000) {
        card.querySelector('.file-card-content').innerHTML =
            '<span style="opacity:.4;font-style:italic;">No preview available</span>';
        return;
    }

    try {
        const res = await fetch(
            `https://raw.githubusercontent.com/${GITHUB_USERNAME}/${file.repoName}/main/${file.path}`
        );
        if (res.ok) {
            const text = await res.text();
            file.previewLoaded = true;
            card.querySelector('.file-card-content').innerText =
                text.substring(0, 120);
        }
    } catch {}
}

/* ===============================
   9. 搜索（保持原语义）
================================ */
document.getElementById('file-search').addEventListener('input', e => {
    const term = e.target.value.toLowerCase();
    const container = document.getElementById('file-cards-container');
    container.innerHTML = "";
    renderIndex = 0;

    const filtered = filesCache.filter(f =>
        f.path.toLowerCase().includes(term)
    );

    filtered.forEach(f => {
        container.insertAdjacentHTML('beforeend', createFileCardHTML(f));
    });

    setupPreviewObserver();
});

/* ===============================
   启动
================================ */
document.addEventListener('DOMContentLoaded', () => {
    type();
    fetchGitHub();
});