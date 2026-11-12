# HSJ 浏览器版数据分析系统

这是 HSJ 数据分析系统浏览器版的完整项目备份，整理日期为 2026 年 11 月 12 日。程序版本为 `0.17-orbit-classic-layout`，包含 2026 年 11 月 11 日的滤波数值修复及当前工作副本中的补丁。

## 运行

Windows 10/11 64 位系统下载并完整解压本仓库后，双击 `Start-BS.cmd` 或 `启动浏览器分析版.cmd`。浏览器将打开 `http://127.0.0.1:8765/desktop`。包内附带 Python 运行环境及离线依赖，普通运行不需要另行安装 Python 或 Node.js。

停止使用 `Stop-BS.cmd`，重启使用 `Restart-BS.cmd`。旧版 DOC/XLS 输出需要本机安装可用的 Word/Excel。更详细的运行和继续开发说明见 [README-FIRST.md](README-FIRST.md) 和 [换电脑运行与继续开发.md](换电脑运行与继续开发.md)。

## 仓库内容

| 目录或文件 | 内容 |
| --- | --- |
| `bs-workbench/` | 全部浏览器版 Python、JavaScript、HTML、CSS 源码，原生协联计算源码和程序、测试、接口文档、交付说明 |
| `bs-workbench/data/` | 当前导入的 SEL/CHD 数据、原有文件 ID 元数据，以及现存批量导出结果 |
| `测试系统实例/` | SEL、CHD、JGR、IAG、ICA、CON 等示例输入 |
| `功能保留清单.md` | 项目功能保留清单 |
| `frontend-review/` | 详细功能基线、功能入口与源码证据、扫描脚本和界面变更记录 |
| `bs-workbench/功能迁移对照表.md` | 功能迁移范围、当前状态和验收规则 |
| `bs-workbench/migration-ledger.json` | 结构化迁移台账及已记录的验收状态 |
| `bs-workbench/功能实现索引.json` | 功能实现的代码入口索引 |
| `ScanAnalysis201204021-20231225/` | 用于迁移对照的原 Windows 桌面版源码和资源 |
| `runtime/` | 便携 Python、已安装依赖及随附许可证 |
| `wheelhouse/` | 固定版本的离线依赖安装包 |
| `python-3.13.13-embed-amd64.zip` | 随原交付包附带的 Python 嵌入式安装包 |
| `GITHUB-UPLOAD-MANIFEST.json` | 本次入库文件的大小和 SHA256 校验清单，不含清单自身 |

## 功能范围

支持 SEL/CHD 文件读取、工况与通道选择、波形与 FFT 频谱、统计量、四类 Butterworth 滤波、游标与标注、多通道叠加、移动趋势、波形长图、三维瀑布图、轴心轨迹、相关分析、动平衡、盘车、效率、协联工况、圆度拟合，以及图像、CSV/JSON、Office 报表和批量导出。前端为原生 JavaScript/Canvas，后端为 FastAPI 和 NumPy/SciPy。

## 当前验证状态

这是开发版本备份，尚未完成整程序用户验收。现有迁移台账登记 70 项要求，完整功能项验收数为 0，另有 1 个局部交互通过用户验收。滤波修复记录报告了回归对照，也保留了部分原版数值差异及旧测试失败的说明，详见 `bs-workbench/滤波数值修复说明_20261111.md` 和 `bs-workbench/qa/`。本次上传只进行文件完整性核对，不扩大原有业务测试或验收结论。

服务默认绑定本机地址，适用于本机单用户使用。浏览器 localStorage/sessionStorage 中的标注、配置和窗口会话不在项目目录内，需要在原浏览器页面另行导出。

本仓库保留原交付清单及各补丁清单作为历史记录；当前文件校验以 `GITHUB-UPLOAD-MANIFEST.json` 为准。未收录可重新生成的 Python 缓存、虚拟环境和临时服务运行记录。第三方组件许可证保留在相应目录中。
