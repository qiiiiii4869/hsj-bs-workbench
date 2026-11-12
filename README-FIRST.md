# HSJ B/S 跨电脑运行与继续开发

打包日期：2026-11-11。程序快照：0.17-orbit-classic-layout。

## 在另一台电脑运行

1. 将 ZIP 完整解压到可写目录，例如 `D:\HSJ-BS`，不要在压缩软件内直接启动。
2. 双击最外层 `Start-BS.cmd` 或 `启动浏览器分析版.cmd`。
3. 浏览器打开 `http://127.0.0.1:8765/desktop`。建议使用 Edge / Chrome，便于使用保存目录功能。
4. 停止服务使用 `Stop-BS.cmd`；修改后端后使用 `Restart-BS.cmd`，修改页面后强制刷新。

包内附带官方 CPython 3.13.13 Windows x64 嵌入式运行环境及全部Python依赖。普通运行不需要另装Python、Node.js或Visual Studio，也不需要联网安装依赖。目标为Windows 10/11 64位，没有制作其他系统版本。

启动器优先使用解压目录的 `runtime/python.exe`，路径按工程位置计算，不依赖原电脑用户名或桌面位置。原电脑的虚拟环境、服务PID和临时日志没有复制，启动后会生成本电脑自己的运行记录。

只有输出真实旧版DOC / XLS需要本机安装可用的Microsoft Word / Excel；DOCX / XLSX、图像和文本直接使用包内依赖。

## 包含内容

- `bs-workbench/`：全部B/S源码、页面、启动脚本、接口文档资源、迁移台账、实现索引和交付说明。
- `bs-workbench/native/`：协联计算源码、构建脚本和已编译的 `cam_math.exe`，运行不需要NI / MFC开发库。
- `bs-workbench/data/`：当前已导入SEL / CHD及UUID元数据，保留文件ID。
- `测试系统实例/`：SEL、CHD、JGR、IAG、ICA、CON等示例输入，保留原相对目录。
- `frontend-review/`：两份功能基线、命令 / 控件 / 事件清单及扫描脚本。
- `ScanAnalysis201204021-20231225/`：继续迁移时用于核对的原C++源码、资源及项目描述；不附带重建整个原桌面程序的NI、BCG和试用工具链。
- `runtime/`：可直接运行的独立Python、项目依赖及相应许可证。
- `wheelhouse/`：固定版本的离线wheel，用于修复和创建开发环境。
- `python-3.13.13-embed-amd64.zip`、`runtime-source.json`：官方原始运行时及来源、SHA256。
- `PACKAGE-MANIFEST.json`：每个交付文件的长度和SHA256。

不包含原电脑 `.venv`、缓存、服务记录、临时日志、旧交付备份、已生成的导出任务文件和原桌面EXE。当前示例和已导入输入文件已包含。浏览器localStorage / sessionStorage中的标注、配置、窗口和会话不是工程文件；如需搬迁，请先在原页面导出分析配置，在新电脑载入。此包不复制浏览器个人资料。

## 继续改进

直接用VS Code或其他编辑器打开最外层目录，修改以下文件：

| 内容 | 文件位置 |
| --- | --- |
| FastAPI服务和路由 | `bs-workbench/server.py`、`api.py` |
| 波形 / 滤波 / 专项计算 | `analysis_service.py`、`analysis.py`、`specialty.py` |
| SEL / CHD解析 | `sel_reader.py`、`data_reader.py` |
| 前端界面与交互 | `bs-workbench/static/` |
| 动平衡 / 轴心轨迹界面 | `static/balance.*`、`static/orbit-layout.*` |
| 报表和批量任务 | `outputs.py`、`special_reports.py`、`export_jobs.py`、`special_jobs.py` |
| 接口说明 / 调试 | 浏览器 `/docs`；定义在 `api_docs.py` |
| 功能范围和验收状态 | `frontend-review/`、`功能实现索引.json`、`migration-ledger.json` |

编辑器解释器可选择 `runtime/python.exe`。如希望用常规虚拟环境扩展开发，先在目标电脑安装Python 3.13 x64，再双击 `Create-Dev-Environment.cmd`。脚本用系统Python创建独立 `.venv`，从wheelhouse离线安装固定版本，不复用原电脑路径。

默认启动器继续使用便携运行时。开发虚拟环境可通过以下命令启动：

```powershell
.\bs-workbench\.venv\Scripts\python.exe .\bs-workbench\server.py --port 8765
```

先停止同电脑另一份服务，避免8765端口冲突。前端是原生JavaScript / Canvas，不需要npm打包。

## 离线修复依赖

双击 `Repair-Dependencies.cmd`，从包内wheelhouse重新安装固定版本。脚本不联网，也不执行分析测试。

`requirements.txt` 是一般依赖范围；`requirements-transfer.lock` 锁定本次21项实际运行依赖；wheelhouse同时附带pip。新增依赖时应更新锁定清单和wheelhouse。

## 修改协联原生计算

日常改Python和页面不必重编译。若修改 `native/cam_math.cpp`，安装Visual Studio C++ Build Tools，在其Developer Command Prompt中运行：

```powershell
.\runtime\python.exe .\bs-workbench\native\build_cam.py
```

脚本使用开发终端的 `cl.exe`，本包不携带原试用工具链。本次打包修正旧编译器的输出文件参数并重新编译，归档包含实际 `cam_math.exe`；未执行算法测试。

## 交付状态

这是当前开发版本的运行与改进快照，不表示所有原版功能已验收。原有缺口说明和验收记录保留。打包仅进行文件收集、依赖安装、协联程序编译及ZIP CRC / 清单核对，没有执行应用业务测试，也没有在另一台电脑上实机验收。

运行时及依赖的许可证随包保留。ZIP旁附整体SHA256校验文件，包内清单用于搬运后核对。
