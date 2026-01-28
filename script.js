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
const phrases = ["工程师", "架构师", "开发者", "Slow is smooth, smooth is fast."]; // 带句号，更像 Apple 文案
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

// --- 4. Home Show 文件卡片生成功能 ---
let allFiles = [];
let displayedFiles = [];

// 判断文件类型
function getFileType(filename, content) {
    const extension = filename.split('.').pop().toLowerCase();
    
    // 二进制文件类型
    const binaryExtensions = [
        'pdf', 'doc', 'docx', 'ppt', 'pptx', 'xls', 'xlsx',
        'exe', 'dll', 'so', 'dylib', 'bin', 'zip', 'rar', '7z',
        'tar', 'gz', 'mp3', 'wav', 'mp4', 'avi', 'mkv'
    ];
    
    if (binaryExtensions.includes(extension)) {
        return 'binary';
    }
    
    // 代码文件类型
    const codeExtensions = [
        'js', 'ts', 'jsx', 'tsx', 'py', 'java', 'c', 'cpp', 'h', 'hpp',
        'go', 'rs', 'php', 'rb', 'swift', 'kt', 'scala', 'lua',
        'sql', 'sh', 'bash', 'zsh', 'fish', 'vim', 'dockerfile',
        'html', 'htm', 'css', 'scss', 'sass', 'less', 'xml', 'json',
        'yaml', 'yml', 'toml', 'ini', 'md', 'markdown', 'txt'
    ];
    
    if (codeExtensions.includes(extension)) {
        return 'code';
    }
    
    // 图片文件类型（通过内容检测）
    const imageExtensions = ['png', 'jpg', 'jpeg', 'gif', 'bmp', 'tiff', 'webp', 'svg'];
    if (imageExtensions.includes(extension)) {
        // 检查内容是否可以访问，如果是空的或无法读取，则认为是二进制
        if (!content || content.length === 0 || content.includes('�') || content.includes('\x00')) {
            return 'binary';
        }
        return 'image';
    }
    
    // 如果是文本扩展名但内容为空，仍视为文本文件
    const textExtensions = ['md', 'markdown', 'txt', 'json', 'xml', 'yaml', 'yml', 'html', 'htm', 'css'];
    if (textExtensions.includes(extension)) {
        return 'code';
    }
    
    return 'binary';
}

// 格式化文件大小
function formatFileSize(bytes) {
    if (bytes < 1024) return bytes + ' B';
    if (bytes < 1024 * 1024) return (bytes / 1024).toFixed(1) + ' KB';
    return (bytes / (1024 * 1024)).toFixed(1) + ' MB';
}

// 生成文件卡片
function generateFileCard(file) {
    const fileType = getFileType(file.name, file.content);
    let icon = '';
    
    switch (fileType) {
        case 'code':
            icon = '<i class="fas fa-code"></i>';
            break;
        case 'binary':
            icon = '<i class="fas fa-file-archive"></i>';
            break;
        case 'image':
            icon = '<i class="fas fa-image"></i>';
            break;
        default:
            icon = '<i class="fas fa-file"></i>';
    }
    
    let contentSnippet = '';
    
    if (fileType !== 'binary' && file.content) {
        // 截取非二进制文件的内容预览
        const plainText = file.content.replace(/\s+/g, ' ').trim();
        contentSnippet = plainText.substring(0, 120);
        if (plainText.length > 120) {
            contentSnippet += '...';
        }
    }
    
    return `
        <div class="file-card glass-card" onclick="window.open('${file.fullUrl}', '_blank')">
            <div class="file-card-icon ${fileType}">${icon}</div>
            <h3 class="file-card-name" title="${file.name}">${file.name}</h3>
            <p class="file-card-path" title="${file.path}">${file.path}</p>
            ${contentSnippet ? `<p class="file-card-content">${contentSnippet}</p>` : '<p class="file-card-bin-only">二进制文件</p>'}
            ${file.size ? `<span class="file-card-size">${formatFileSize(file.size)}</span>` : ''}
        </div>
    `;
}

// 获取项目文件（通过GitHub API tree）
async function getRepoFiles() {
    try {
        // 获取仓库内容
        const reposRes = await fetch(`https://api.github.com/users/${GITHUB_USERNAME}/repos?sort=pushed&per_page=1`);
        if (!reposRes.ok) throw new Error('无法获取仓库列表');
        
        const repos = await reposRes.json();
        if (repos.length === 0) throw new Error('未找到仓库');
        
        // 获取默认分支名称
        const repoRes = await fetch(`https://api.github.com/repos/${GITHUB_USERNAME}/${repos[0].name}`);
        if (!repoRes.ok) throw new Error('无法获取仓库信息');
        
        const repo = await repoRes.json();
        const defaultBranch = repo.default_branch;
        
        // 获取仓库树
        const treeRes = await fetch(`https://api.github.com/repos/${GITHUB_USERNAME}/${repos[0].name}/git/trees/${defaultBranch}?recursive=1`);
        if (!treeRes.ok) throw new Error('无法获取文件树');
        
        const treeData = await treeRes.json();
        const files = treeData.tree.filter(item => item.type === 'blob');
        
        // 获取文件内容
        for (const file of files) {
            const contentRes = await fetch(`https://raw.githubusercontent.com/${GITHUB_USERNAME}/${repos[0].name}/${defaultBranch}/${file.path}`);
            if (contentRes.ok) {
                const content = await contentRes.text();
                allFiles.push({
                    name: file.path.split('/').pop(),
                    path: file.path,
                    content: content,
                    size: file.size,
                    fullUrl: `https://github.com/${GITHUB_USERNAME}/${repos[0].name}/blob/${defaultBranch}/${file.path}`
                });
            } else {
                // 如果获取内容失败，可能是二进制文件
                allFiles.push({
                    name: file.path.split('/').pop(),
                    path: file.path,
                    content: '',
                    size: file.size,
                    fullUrl: `https://github.com/${GITHUB_USERNAME}/${repos[0].name}/blob/${defaultBranch}/${file.path}`
                });
            }
            
            // 添加延迟，避免API限制
            await new Promise(resolve => setTimeout(resolve, 100));
        }
        
    } catch (error) {
        console.error('获取文件失败:', error);
        alert('获取文件失败: ' + error.message);
    }
}

// 生成文件卡片
async function generateFileCards() {
    const container = document.getElementById('file-cards-container');
    const generateBtn = document.getElementById('generate-cards');
    
    // 显示加载状态
    generateBtn.innerHTML = '<i class="fas fa-spinner fa-spin"></i> 生成中...';
    generateBtn.disabled = true;
    
    // 如果还没有获取文件，先获取文件
    if (allFiles.length === 0) {
        await getRepoFiles();
    }
    
    // 随机选择最多20个文件进行展示
    const shuffledFiles = [...allFiles].sort(() => 0.5 - Math.random());
    displayedFiles = shuffledFiles.slice(0, 20);
    
    // 生成卡片HTML
    container.innerHTML = displayedFiles.map(file => generateFileCard(file)).join('');
    
    // 如果有搜索词，同时执行搜索
    const searchInput = document.getElementById('file-search');
    if (searchInput.value) {
        searchFiles(searchInput.value);
    } else {
        // 恢复按钮状态
        generateBtn.innerHTML = '<i class="fas fa-sync-alt"></i> 重新生成';
        generateBtn.disabled = false;
    }
}

// 绑定生成按钮事件
document.getElementById('generate-cards').addEventListener('click', generateFileCards);

// 绑定搜索输入事件
document.getElementById('file-search').addEventListener('input', (e) => {
    searchFiles(e.target.value);
});

// 搜索文件卡片
function searchFiles(searchTerm) {
    const container = document.getElementById('file-cards-container');
    
    if (!searchTerm) {
        // 如果搜索框为空，显示所有当前显示的文件
        container.innerHTML = displayedFiles.map(file => generateFileCard(file)).join('');
        return;
    }
    
    // 过滤文件
    const filteredFiles = allFiles.filter(file => 
        file.name.toLowerCase().includes(searchTerm.toLowerCase()) ||
        file.path.toLowerCase().includes(searchTerm.toLowerCase()) ||
        (file.content && file.content.toLowerCase().includes(searchTerm.toLowerCase()))
    );
    
    // 显示搜索结果
    container.innerHTML = filteredFiles.map(file => generateFileCard(file)).join('');
    
    if (filteredFiles.length === 0) {
        container.innerHTML = '<p style="text-align: center; color: var(--text-secondary); padding: 2rem;">未找到匹配的文件</p>';
    }
}

// 页面加载时自动生成一次文件卡片
document.addEventListener('DOMContentLoaded', () => {
    // 延迟执行，确保页面完全加载
    setTimeout(() => {
        generateFileCards();
    }, 2000);
});

loadGitHubData();
