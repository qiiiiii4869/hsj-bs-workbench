'use strict';
(() => {
  const status = document.getElementById('docs-status');
  const button = document.getElementById('docs-session');
  let ui;
  async function refreshOverview() {
    const inventory = document.getElementById('docs-inventory');
    try {
      const response = await fetch('/api/schema', {cache: 'no-store'});
      if (!response.ok) throw new Error('接口定义加载失败');
      const schema = await response.json();
      const count = Object.values(schema.paths).reduce((n, methods) => n + Object.keys(methods).filter(m => ['get','post'].includes(m)).length, 0);
      const examples = schema.paths['/api/special'].post.requestBody.content['application/json'].examples;
      inventory.textContent = `${count} 个接口（含 OpenAPI 定义） · ${Object.keys(examples).length} 类专项模块 · 请求参数、响应结构和下载格式已列出`;
    } catch (error) { inventory.textContent = error.message; }
  }
  async function connectSession() {
    button.disabled = true;
    status.classList.remove('error');
    status.textContent = '正在读取本机会话…';
    try {
      const response = await fetch('/api/bootstrap', {cache: 'no-store', credentials: 'same-origin'});
      if (!response.ok) throw new Error('本机服务连接失败');
      const data = await response.json();
      if (!data.token) throw new Error('服务没有返回会话令牌');
      ui.preauthorizeApiKey('HSJSession', data.token);
      status.textContent = '本机会话已连接 · POST 接口已授权';
    } catch (error) {
      status.classList.add('error');
      status.textContent = error.message;
    } finally {
      button.disabled = false;
    }
  }
  button.addEventListener('click', connectSession);
  if (typeof SwaggerUIBundle !== 'function') {
    status.classList.add('error');
    status.textContent = '本地文档资源加载失败，请刷新页面。';
    button.disabled = true;
    return;
  }
  ui = SwaggerUIBundle({
    url: '/api/schema', dom_id: '#swagger-ui',
    presets: [SwaggerUIBundle.presets.apis], layout: 'BaseLayout',
    validatorUrl: null, queryConfigEnabled: false, persistAuthorization: false,
    deepLinking: true, docExpansion: 'list', defaultModelsExpandDepth: 1,
    defaultModelExpandDepth: 2, filter: true, displayOperationId: true,
    supportedSubmitMethods: ['get', 'post'], displayRequestDuration: true,
    requestInterceptor(request) {
      for (const [name, value] of Object.entries(request.headers || {})) {
        if (['x-filename', 'x-title', 'x-unit'].includes(name.toLowerCase()) && /[^\x00-\x7f]/.test(String(value))) {
          request.headers[name] = encodeURIComponent(value);
        }
      }
      return request;
    },
    onComplete() { connectSession(); refreshOverview(); },
  });
})();
