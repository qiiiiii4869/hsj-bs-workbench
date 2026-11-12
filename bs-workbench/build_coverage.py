"""Create a separate migration ledger; never overwrite the desktop baseline."""
from pathlib import Path
import hashlib
import json
import re

ROOT = Path(__file__).resolve().parent
BASE = ROOT.parent / 'frontend-review'
partial = {
    'W-12': ('添加移动趋势 / 依据、窗宽、步长与颜色 / 曲线管理和导出', '0.8 接入平均/中间/有效/峰峰值趋势、采样点参数、原版时间定位、颜色显隐/移除、拾取与标注、CSV/PNG/JSON；有效值保留原代码口径并提供显式逐窗选项，尾部不擅自补零，未测试待验收'),
    'W-11': ('添加通道数据 / 颜色 / 曲线显隐与移除 / 捡取目标', '0.7 接入同文件同工况原始波形叠加、各自采样率对齐、显式单位、不自动换算、颜色与显隐、逐曲线游标/标注、CSV与PNG/JSON记录；未运行测试，待用户验收'),
    'W-09': ('键相整周期复选框 / 实际取点区间', '补入 SEL 编码键相读取、原版默认整周期、首尾键相取点；键相信号图示及全部交互联动仍待迁移，数值待用户验收'),
    'W-01': ('显示模式：波形 / 频谱 / 波形与频谱', '三种模式已接入，同一计算结果切换；待原版交互对照'),
    'W-06': ('峰峰值置信度 95% / 97% / 100%', '按原 GetConf 排序下标规则实现，选择与导出同步；待原版运行结果对照'),
    'W-07': ('修改图表标题', '支持确定与取消，页面及 CSV/JSON 标题同步；原版图像导出标题待迁移'),
    'W-13': ('设置波形滤波 / 记录与对照 / 撤销上次 / 恢复原始 / 数据导出', '滤波操作流程已实现并交用户验收：四类 Butterworth、阶数 1–100、原版前补规则、分块连续处理、每工况通道保存设置、原始叠加对照和导出；0.3-filter 按用户要求未运行测试，NI 数值一致性待用户验收'),
    'F-01': ('导入 SEL / 打开所选文件', '已支持真实文件与取消选择；待对照全部工况、参数及异常文件'),
    'F-03': ('工况下拉框 / 通道列表', '已实现切换、搜索；原树层级、多视图双击仍待迁移'),
    'W-02': ('波形 X/Y/XY 框选、X/XY 平移、撤销、重绘', '0.6 接入框选放大、拖动与取消、独立坐标历史、逐步撤销、全通道重绘；按当前视区分析为显式按钮，原版缩放自动联动统计/FFT 仍待补齐，未运行测试'),
    'W-03': ('波形图操作模式 / 右键：捡取浮动点、捡取临近点', '浮动坐标与吸附完整计算数据的临近点已实现；单击/拖动定位、双向捡取线、点序号及键盘逐点移动；原版 NI 交互一致性待用户验收'),
    'W-04': ('频谱 X/XY 框选、X 平移、临近点、撤销、重绘', '0.6 接入 X/XY 框选、逐步撤销、重绘及低频选择的历史；完整谱线读数和细节读取保留；NI 坐标行为及全部联动待用户验收，未运行测试'),
    'W-08': ('添加标注 / 移动位置 / 清除标注 / 右键 / PNG 与 JSON 导出', '自定义文字绑定真实采样/频谱点、修改/删除/拖动、取消、按分析参数保存及导出已实现；ID_Annotations 的甩负荷专用标注未与自定义标注合并，仍待专项模块迁移'),
    'W-05': ('顶部指标 / 右侧分析结果', '已实现统计及 8 条谱线；未与 NI 运行基线逐项验收'),
    'W-10': ('起始时间 / 结束时间', '明确采用 [start,end)；原版边界取点语义待核对'),
    'E-02': ('当前/全时段波形 CSV / 频谱 CSV', '全时段及当前时段已有独立入口，滤波/置信度/标题写入记录；原 TXT 格式及取点边界待对照'),
    'K-03': ('原生按钮、输入和下拉框', '可用 Tab 导航；菜单助记键与全部业务入口待迁移'),
    'K-04': ('参数表单', 'Enter 提交已实现；45 个原对话框取消/焦点语义待逐项迁移'),
    'K-05': ('Canvas 游标、标注与图中右键', '已接入浮动/临近点、标签命中/拖动、右键操作；全部原版周期选择与专项事件仍待迁移，本轮未测试'),
    'K-06': ('加载/计算时禁用与错误状态', '已实现首条链路状态；多视图命令路由与全模块状态未迁移'),
}
command_partial = {
    'ID_AddMobTrend':'添加移动趋势弹窗：计算依据、窗口宽度/步长（点）、颜色，确定/取消',
    'ID_AddChnData':'添加通道数据弹窗，选择通道及颜色，叠加当前工况的原始波形',
    'ID_XZOOM':'波形框选放大 X', 'ID_YZOOM':'波形框选放大 Y', 'ID_Zoom':'波形框选放大 XY',
    'ID_Moving':'波形 XY 平移', 'ID_UnZoom':'波形逐步撤销缩放/平移', 'ID_resume':'重新分析完整通道并重绘波形',
    'ID_XZOOMF':'频谱框选放大 X', 'ID_ZoomF':'频谱框选放大 XY', 'ID_XMovingF':'频谱 X 平移',
    'ID_UnZoomF':'频谱逐步撤销缩放/平移', 'ID_resumeF':'重绘当前分析区间的全部频谱',
    'ID_FullPeriodFFT':'键相整周期复选框，默认沿用原版；无键相时禁用',
    'ID_WaveGraph':'仅波形显示', 'ID_FRAQGRAPH':'仅频谱显示', 'ID_WAVEFRAQGRAP':'波形与频谱显示',
    'ID_NINEFIVE':'95% 峰峰值', 'ID_NINESEVEN':'97% 峰峰值', 'ID_ONEHUNDRED':'100% 峰峰值',
    'ID_SetTitle':'修改图表标题', 'ID_WaveFilter':'波形滤波：四类 Butterworth、连续应用、分块处理、记录与原始叠加对照（交用户验收）',
    'ID_ResumeFilter':'恢复原始数据', 'ID_TextSave':'全时段波形 CSV（原 TXT 格式待对照）',
    'ID_FILEOPENDATA':'导入 SEL / 打开文件',
    'ID_CURR_TEXTDATA':'当前区间波形 CSV（格式与边界待对照）',
    'ID_FREQ_TEXTDATA':'当前区间频谱 CSV（格式待对照）',
    'ID__TIME_SPAN_NON_FUUL_PERIOD':'分析起止时间（边界待对照）',
    'ID_CurrorFloat':'波形操作模式 / 右键：捡取浮动点',
    'ID_CurrorNearestPoint':'波形操作模式 / 右键：捡取临近点（完整计算采样点）',
    'ID_CurrorNearestPointF':'频谱操作模式 / 右键：捡取临近点（完整频谱点）',
    'ID_ANNOTATION':'临近点定位后添加文字标注，支持取消与修改',
    'ID__CANCEL_ALL_ANNOTATION':'清除当前图标注',
    'ID_ADJUST_ANNOTATION_PLACE':'移动标注位置模式与拖动',
}


partial.update({
    'L-01':('波形长图→新建波形长图','0.10 接入工况/通道、全选/反选、独立/分组模式、A4/A3/自定义画布、行列与所占格数；保留原默认值，未测试待用户验收'),
    'L-02':('绘图区域与网格 / 添加信号 / 添加分组','0.10 接入几何布局、组内最多10曲线、共享纵轴、原始采样率和单位、数据对应关系，未测试待验收'),
    'L-03':('标题区右键→设置标题','按标题区命中提供独立右键，确定/取消及图像输出标题接入，待验收'),
    'L-04':('X轴时间与列数 / 时间轴区右键','起止秒数、列数、重读原始时段与重绘接入；不按屏幕宽度篡改时间，待验收'),
    'L-05':('纵轴区右键→修改/删除','组内信号选择、波形名、线宽/颜色、起止网格、上下限、删除确认/取消及后续组收拢选项接入，待验收'),
    'L-06':('左侧分组树右键→添加分组/信号','原TPM_RETURNCMD直接分派的功能已提供独立树右键和按钮入口，未遗漏为无绑定项，待验收'),
    'L-07':('纵轴拖动 / Shift调高 / 滚动 / 图像保存','纵轴移动吸附网格、Shift调整占用高度、Esc取消、双向滚动、整图/视区五格式与SVG导出接入；未测试待验收'),
    'F-02':('文件属性按钮','已接入文件路径/时间、SHA256、SEL 采样配置、工况及通道完整元数据；展示为浏览器属性窗口，待验收'),
    'F-05':('载入 CHD 数据 FFT','数值波形导入、显式采样频率、标题、单位和颜色；无键相时非整周期，待验收'),
    'F-06':('保存运行日志','分析/状态与输出记录可下载本会话 TXT，待验收'),
    'W-02':('X/Y/XY 框选、XY 平移、撤销、重绘、自动视区分析','0.9 默认联动统计与 FFT，保留完整波形用于外层撤销；无效区间提示并回退，未测试待验收'),
    'W-08':('自定义标注 / 甩负荷专用标注 / 拖动 / 清除 / 输出','两类功能已分开接入；0.6.1 的直接拖动已单独验收。甩负荷旧代码取消/文字位置异常作明确调整记录，整体待验收'),
    'W-09':('整周期开关 / 键相点显隐 / 图表联动','0.9 接入键相显示、视区联动；默认原版整周期，未测试待验收'),
    'W-10':('设置分析时段（非整周期）弹窗 / 右键','起止输入、确定后重算及取消保留、边界校验已接入，待验收'),
    'W-14':('甩负荷窗口与专用标注','1–10000 点窗口、基准/最终时刻、上升/下降、基准/极值/变化率、专用标注与趋势管理接入；原缺陷调整及尾部规则见交付文档，待验收'),
    'E-01':('数据/图像/报表输出','当前视图/完整分析区间、单波形/单频谱/双图、BMP/JPG/GIF/PNG/TIFF、自定义尺寸、取消，已接入待验收'),
    'E-02':('当前/全时段波形 CHD、前8频谱 CHD、完整CSV','保留原六位小数波形数值行与频谱表编码；三种范围分别提供，待验收'),
    'E-03':('单通道 Word/Excel 分析报表','DOCX/XLSX 包含统计、图像、谱峰、标题/单位/参数；Excel 另含完整分析数据，原二进制DOC/XLS及原模板不宣称等价，待验收'),
    'E-04':('批量输出→批量图像','工况/通道勾选、五格式、自定义尺寸、频率范围、CON、整周期、后台进度/取消/ZIP与错误清单，待验收'),
    'E-05':('批量输出→通道数据 CHD','按勾选工况通道导出原值格式，独立文件与清单打包ZIP，待验收'),
    'E-06':('批量输出→汇总 Excel','勾选范围、CON、整周期、参数、统计/8谱峰与图像汇总 XLSX，待验收'),
    'E-07':('批量输出→汇总 Word','勾选范围、CON、整周期、逐通道图表与统计汇总 DOCX，待验收'),
    'E-08':('批量窗口工况分组 / 勾选 / 全选 / 反选','原生复选框支持鼠标与空格，稳定保存工况通道索引、分类选择和逐项选择，待验收'),
})
command_partial.update({
    'ID_StripChartGen':'波形长图页面→新建波形长图',
    'ID_MENUITEMSetRange':'长图工具栏/图区右键→绘图区域与网格',
    'ID_MENUITEMAddSignal':'长图工具栏/图区右键→添加信号',
    'ID_MENUITEMGroup':'长图工具栏/图区右键→添加分组',
    'ID_MENUITEMSetTitle':'长图标题区右键→设置标题',
    'ID_MENUITEMSetXAxis':'长图时间轴右键→时间与列数',
    'ID_MENUITEMSetAxis':'长图纵轴右键→波形与坐标信息',
    'ID_MENUITEMDeleteSign':'长图纵轴右键→删除信号/分组，带确认取消',
    'ID_MENUITEMAddGroup':'长图分组树右键→添加分组',
    'ID_MENUITEMAddSign':'长图分组树右键→向分组添加信号',
    'ID_MENUITEMSaveToImage':'长图工具栏/图区右键→完整长图或当前视区图像',
    'ID_KeyPhaseVisual':'键相点显隐复选框', 'ID_Annotations':'甩负荷窗口与专用标注（独立于自定义文字标注）',
    'ID_WINDOW_SET':'甩负荷窗口宽度、基准/最终时刻、上升/下降模式',
    'ID_SaveSingleView':'输出窗口→单个波形图', 'ID_SaveAllView':'输出窗口→波形与频谱全图',
    'ID_CustomSaveAllView':'输出窗口→自定义尺寸和格式',
    'ID_TextSave':'原格式全时段波形 CHD', 'ID_CURR_TEXTDATA':'原格式当前时段波形 CHD',
    'ID_FREQ_TEXTDATA':'原格式前8谱峰频率/幅值/相位 CHD',
    'ID_WordReport':'输出窗口→Word DOCX 报表', 'ID_ExcelReport':'输出窗口→Excel XLSX 报表',
    'ID_SAVEPICBAT':'批量窗口→图像', 'ID_SAVETXTDATA':'批量窗口→通道数据CHD',
    'ID_SAVEEXCELDATA':'批量窗口→Excel汇总', 'ID_SAVEWORDDATA':'批量窗口→Word汇总',
    'ID_FILEPproperty':'文件属性窗口','ID_LODDATA_FFT':'载入 CHD 数据 FFT','ID_SAVERECORDTOTEXT':'保存运行日志',
})


def build():
    baseline = json.loads((BASE/'frontend-inventory.json').read_text('utf-8'))
    features=[]
    for line in (BASE/'功能保留清单.md').read_text('utf-8').splitlines():
        if not re.match(r'^\| [FWELDACTBJOK]-\d\d \|',line):continue
        columns=[c.strip() for c in line.strip('|').split('|')]
        key,label,evidence,acceptance=columns[:4]
        entry,note=partial.get(key,('待实现','必须保留，未获得删除或替换授权'))
        features.append(dict(id=key,name=label,source=evidence,acceptance=acceptance,
                             status='已实现，待用户验收' if key=='W-13' else '部分实现，待对照' if key in partial else '待迁移',web_entry=entry,note=note,accepted=False))
    commands=[]
    for item in baseline['commands']:
        match=next((command_partial[k] for k in item['ids'] if k in command_partial),None)
        is_filter=any(k in item['ids'] for k in ('ID_WaveFilter','ID_ResumeFilter'))
        commands.append(dict(item, migration_status='已实现，待用户验收' if is_filter else '部分实现，待对照' if match else '待迁移',web_entry=match or '待实现',accepted=False))
    dialogs=[dict(r,review_id=f'DLG-{i:02}',migration_status='待迁移',accepted=False)
             for i,r in enumerate((r for r in baseline['resources'] if r['type'].startswith('DIALOG')),1)]
    events=[dict(e,review_id=f'EVT-{i:03}',migration_status='待迁移',accepted=False) for i,e in enumerate(baseline['events'],1)]
    acceptance_file=ROOT/'acceptance-records.json'
    user_acceptance=json.loads(acceptance_file.read_text('utf-8'))['records'] if acceptance_file.exists() else []
    ledger=dict(date='2026-11-08',baseline_sha256=hashlib.sha256((BASE/'frontend-inventory.json').read_bytes()).hexdigest(),
                policy='完整替代的准入条件：逐项迁移并验收；部分实现不等于保留完成；不静默删除历史/框架/待确认项。',
                accepted_count=0,accepted_subscope_count=len(user_acceptance),user_acceptance=user_acceptance,
                complete_replacement_ready=False,baseline_summary=baseline['summary'],
                features=features,commands=commands,dialogs=dialogs,events=events,
                resources=baseline['resources'],bindings=baseline['bindings'],direct_dispatches=baseline['direct_dispatches'],
                hooks=baseline['hooks'],commented_command_lines=baseline['commented_command_lines'])
    assert len(commands)==139 and len(dialogs)==45 and len(events)==276
    assert sum(len(d['entries']) for d in dialogs)==650
    (ROOT/'migration-ledger.json').write_text(json.dumps(ledger,ensure_ascii=False,indent=2),'utf-8')
    lines=['# B/S 功能迁移对照表','','基线：frontend-review/功能保留清单.md 及 frontend-inventory.json。',
           '',f'共 {len(features)} 项分组要求、139 个 CMD、45 个窗口/650 个控件、276 个事件；196 个菜单入口、89 个工具栏按钮均随原始证据保留。',
           '', '**已通过完整替代验收：0。当前不能替代桌面版。**',
           '',f'用户已验收 {len(user_acceptance)} 个具体操作范围，详见 acceptance-records.json；不会据此将整个功能组记为通过。',
           '', '结构化逐项记录见 migration-ledger.json。原始资源入口、绑定、直接右键分支、历史命令全部保留。',
           '', '| 原编号 | 必须保留的操作 | B/S 当前入口 | 状态 | 未完成项 / 验收要求 |','|---|---|---|---|---|']
    for row in features:
        lines.append('| '+' | '.join(str(row[k]).replace('|','／') for k in ('id','name','web_entry','status','note'))+' |')
    lines += ['', '## 验收规则','','1. 按相同文件 SHA256、工况、通道、区间和参数对照原程序，记录数值误差与导出内容。',
              '2. 逐一验证菜单、工具栏、右键、窗口控件、鼠标及键盘操作，保留原 CMD/DLG/EVT 关联。',
              '3. 原版待确认项须先做原版运行验证；不得记成迁移通过，也不得自行取消。',
              '4. 文件/参数/计算/交互/导出均通过后，才能将 accepted 改为 true。',
              '5. 主题、标签、多窗口等交互若要更换方案，先取得用户明确认可。',
              '', '## 后续迁移顺序','','优先补齐 W/F/E 的完整闭环及原版结果基线；随后迁移长图、三维、轴心轨迹、相关、趋势；再迁移动平衡、盘车、效率、协联、圆度及批量报表。顺序不改变任何功能的保留要求。']
    (ROOT/'功能迁移对照表.md').write_text('\n'.join(lines)+'\n','utf-8')
    page_lines=['# 波形与频谱逐项迁移核对','','当前版本：0.9-wave-workbench，2026-11-08。依据用户指定的两份功能基线；未运行测试，已接入不代表已通过验收。',
                '', 'W-01 至 W-14 与 E-01 至 E-08 的实现入口如下；原 CMD/DLG/EVT 及资源证据完整保留于 migration-ledger.json。',
                '', '| 原编号 | 原功能要求 | 当前网页入口 | 实现与兼容边界 |','|---|---|---|---|']
    for row in features:
        if row['id'].startswith(('W-','E-')):
            page_lines.append('| '+' | '.join(str(row[k]).replace('|','／') for k in ('id','name','web_entry','note'))+' |')
    page_lines+=['','用户已验收的具体操作范围单独保存在 acceptance-records.json，本轮没有将该范围扩展为整页或整程序验收。',
                 '', '完整操作和明确的行为调整见《波形与频谱页面交付说明》；DOCX/XLSX、ZIP 下载和甩负荷旧缺陷处理均已列出，不宣称与原模板或旧控件逐字节等价。',
                 '', '其他页面（长图、三维、轴心轨迹、相关、动平衡等）仍按全程序清单迁移，不因本页实现而删除。']
    (ROOT/'波形频谱逐项迁移核对.md').write_text('\n'.join(page_lines)+'\n','utf-8')
    strip_lines=['# 波形长图逐项迁移核对','','0.10-strip-chart，2026-11-08。依据两份原始功能基线；本轮未运行测试，均交用户验收。',
                 '', '| 原编号 | 原功能 | 网页入口 | 实现与验收状态 |','|---|---|---|---|']
    for row in features:
        if row['id'].startswith('L-'):strip_lines.append('| '+' | '.join(str(row[k]).replace('|','／') for k in ('id','name','web_entry','note'))+' |')
    strip_lines+=['','原始 CMD/DLG/EVT 与资源证据保持在 migration-ledger.json；具体默认参数、数据口径和浏览器实现边界见《波形长图交付说明》。',
                  '', '波形与频谱页的验收状态与长图实现分开维护，不互相替代。']
    (ROOT/'波形长图逐项迁移核对.md').write_text('\n'.join(strip_lines)+'\n','utf-8')
    print(json.dumps(dict(features=len(features),commands=len(commands),dialogs=len(dialogs),events=len(events),accepted=0)))


if __name__=='__main__':build()
