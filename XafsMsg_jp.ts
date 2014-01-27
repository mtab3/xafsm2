<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="ja_JP">
<context>
    <name>AUnit</name>
    <message>
        <source> TP[%1] ID[%2] NM[%3] DR[%4] Ch[%5] DC[%6]  UT[%7] PU[%8] CT[%9] MaV[%10] MiV[%11] </source>
        <translation>TP[%1] ID[%2] NM[%3] DR[%4] Ch[%5] DC[%6]  UT[%7] PU[%8] CT[%9] MaV[%10] MiV[%11]</translation>
    </message>
</context>
<context>
    <name>Conditions</name>
    <message>
        <source>Frame</source>
        <translation>Frame</translation>
    </message>
    <message>
        <source>Encorder is always used for Recorded Th</source>
        <translation>θとして常にエンコーダの値を記録する</translation>
    </message>
    <message>
        <source>Selected Encorder or &apos;calc from PM&apos; is used</source>
        <translation>エンコーダかパルスモータの換算値かを選択する</translation>
    </message>
    <message>
        <source>No normalization</source>
        <translation type="obsolete">計測時間の補正無し</translation>
    </message>
    <message>
        <source>No normalization    (now &apos;No&apos; shold be selected always)</source>
        <translation>規格化なし(今は、つねにこれを選んで下さい)</translation>
    </message>
    <message>
        <source>Normalize the SSD-measured value by RealTime</source>
        <translation>SSDの測定値を「RealTime」で規格化する</translation>
    </message>
    <message>
        <source>Normalize the SSD-measured value by LiveTime</source>
        <translation>SSDの測定値を「LiveTime」で規格化する</translation>
    </message>
    <message>
        <source>Some additional information is recorded 
with usual 9809 format datas.</source>
        <translation>9809形式の通常のデータに加えて、追加の情報を行末に記録する(エンコーダ読み、パルスモータ換算両方の角度とエネルギー)。</translation>
    </message>
    <message>
        <source>Record Addicional Informations in &quot;.dat&quot; XAFS file</source>
        <translation>XAFS 測定の &quot;.dat&quot; ファイルに付加的な情報を含める。</translation>
    </message>
    <message>
        <source>Make &quot;.info&quot; file to record further information on XAFS</source>
        <translation>XAFS測定の際 &quot;.info&quot; ファイルを作る。</translation>
    </message>
    <message>
        <source>Record Addicional Informations for XAFS measurement</source>
        <translation type="obsolete">XAFS測定の記録ファイルに付加情報を含める</translation>
    </message>
    <message>
        <source>Use 1303 format to record XAFS measurement</source>
        <translation type="obsolete">XAFS測定の記録ファイルに 1303 フォーマットを使用する</translation>
    </message>
    <message>
        <source>Record All SSD Chs in ssd.dat while Monitoring</source>
        <translation>モニター時に SSD の全チャンネルのデータを記録する(ssd.dat)</translation>
    </message>
    <message>
        <source>Emergent choice.
To handle the malfunction of several SSD Chs,
i.e. sometimes the count rates dorop to zero,
a virtual SSD Ch is used.
The channel gives value to compensate 
the out-puts of deading channels.</source>
        <translation type="obsolete">緊急避難的な選択肢。
SSDの一部のチャンネルに、
カウントが突然0になる不具合があるが、
これに対応する仮想チャンネルを使う。
仮想チャンネルは、0になったチャンネルの出力を
補う値を持つように働く。</translation>
    </message>
    <message>
        <source>Virtual 20th SSD Ch is used</source>
        <translation type="obsolete">20番目の仮想 SSD チャンネルを使用する</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;The 2nd raw in data-file of normal XAFS measurement will be encorder-reading value.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>ステップスキャンの際のデータファイルの２列めの数値が、必ずエンコーダ読みの角度になります。</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;The 2nd raw in data-file of normal XAFS measurement will be encorder-reading values or values calculated from the answer of the puls-motor-controller.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>ステップスキャンの際のデータファイルの２列めの数値を、エンコーダ読みの角度にするか、パルスモータコントーローラから読んだパルス値を角度に換算したものにするか、を選択可能。</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;When toggle-on the button, the difference between the encorder-reading value and the value calculated from the answer of the pulse-motor-controller is recorded, and the difference will be used to decide the monochromator position to go.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="obsolete">このボタンをオンにすると、その時点でのエンコーダ読みの角度とパルスモータコントローラのパルスから計算した角度との差が記録され、分光器を移動する際の移動先の計算に使われるようになります。</translation>
    </message>
    <message>
        <source>Use fixed delta-value between PM and true angle.</source>
        <translation type="obsolete">分光器移動のパルスの計算で、真値との差を固定値にする</translation>
    </message>
    <message>
        <source>Set</source>
        <translation>設定</translation>
    </message>
    <message>
        <source>Calibrate DXM controlling PMC</source>
        <translation>分光器コントロールPMCの較正</translation>
    </message>
    <message>
        <source>in degree</source>
        <translation>角度</translation>
    </message>
    <message>
        <source>in selected unit for display</source>
        <translation>表示用に選択された単位</translation>
    </message>
    <message>
        <source>Measurement will conduct at even interval</source>
        <translation>XAFS測定を下記の単位で等間隔に刻む</translation>
    </message>
    <message>
        <source>Infos</source>
        <translation>バージョン情報</translation>
    </message>
    <message>
        <source>Calibrate the original point of the Pulse Moter Controller which controll the DXM.
The original point will be changed as the angle calculated from the pulse will be equal to the angle observed by the encorder. </source>
        <translation>パルス値から計算した分光器角度と、エンコーダで見た分光器角度が一致するよう、分光器を制御しているパルスモータコントローラの原点を較正します。</translation>
    </message>
    <message>
        <source>When the Th of the DXM is changed, Delta Theta 1 is changed according to the presented tuning table.    A new line can be add to the table, by using the &quot;DTh1 &apos;Set&apos;&quot; button.
</source>
        <translation>このボタンをチェックすると、分光器の角度(θ)を変更する際、予め登録されたテーブルに従って⊿θ1を調整します。</translation>
    </message>
    <message>
        <source>Use Dth1 tuning table</source>
        <translation>テーブルを使用して⊿θ1をチューニングする</translation>
    </message>
    <message>
        <source>The tuning table will be saved with the file name of &quot;date-TuningTable.txt&quot; style, like as, &quot;130722-TuningTable.txt&quot;.     When you want to make XafsM2 read the table, rename the file to TuningTable.txt.   Then XafsM2 read the file on start up.</source>
        <translation>チューニング用のテールをセーブします。その際ファイル名は「日付-TuningTable.txt」となります(例えば 130722-TuningTable.txt)。  セーブしたテーブルをXafsM2に読ませる場合、名前から日付を消して「TuningTable.txt」とすると、XafsM2の起動時に読まれます。</translation>
    </message>
    <message>
        <source>Save The Table</source>
        <translation>テーブル保存</translation>
    </message>
    <message>
        <source>Pop up a window to show the tuning table of the Delta Theta 1.    The table can be edited in the window.</source>
        <translation>⊿θ1のチューニングテーブルを表示します。現れた表は編集可能です。</translation>
    </message>
    <message>
        <source>Show The Table</source>
        <translation>テーブル表示</translation>
    </message>
    <message>
        <source>Type of 1st order Differential</source>
        <translation>1階微分のタイプ</translation>
    </message>
    <message>
        <source>Type 1</source>
        <translation>タイプ 1 (平滑弱)</translation>
    </message>
    <message>
        <source>Type 2</source>
        <translation>タイプ 2 (平滑中)</translation>
    </message>
    <message>
        <source>Type 3</source>
        <translation>タイプ 3 (平滑強)</translation>
    </message>
    <message>
        <source>Type of 2nd order Differential</source>
        <translation>2階微分のタイプ</translation>
    </message>
</context>
<context>
    <name>DarkTable</name>
    <message>
        <source>Frame</source>
        <translation>バックグラウンド</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>閉じる</translation>
    </message>
    <message>
        <source>Sensor Name</source>
        <translation>検出器名</translation>
    </message>
    <message>
        <source>Dark Value</source>
        <translation>バックグラウンド値</translation>
    </message>
</context>
<context>
    <name>Data</name>
    <message>
        <source>Frame</source>
        <translation>Frame</translation>
    </message>
    <message>
        <source>Show</source>
        <translation>表示</translation>
    </message>
    <message>
        <source>  Type : </source>
        <translation>タイプ : </translation>
    </message>
    <message>
        <source>Select</source>
        <translation>選択</translation>
    </message>
    <message>
        <source>Measured</source>
        <translation>XAFS測定</translation>
    </message>
    <message>
        <source>Scaned</source>
        <translation>スキャン</translation>
    </message>
    <message>
        <source>Monitored</source>
        <translation>モニタ</translation>
    </message>
    <message>
        <source>MCA</source>
        <translation>MCA(SSD)</translation>
    </message>
    <message>
        <source>Can not open the file %1.</source>
        <translation>ファイル [%1] を開けません。</translation>
    </message>
    <message>
        <source>The file %1 is empty.</source>
        <translation>ファイル [%1] は空です。</translation>
    </message>
    <message>
        <source>The file %1 is not avaliable to show.</source>
        <translation>ファイル [%1] は表示できません。</translation>
    </message>
    <message>
        <source>No View is available.</source>
        <translation type="obsolete">使用可能な表示領域がありません。</translation>
    </message>
</context>
<context>
    <name>FrameWGrid</name>
    <message>
        <source>Frame</source>
        <translation type="obsolete">Frame</translation>
    </message>
</context>
<context>
    <name>MCAView</name>
    <message>
        <source>Frame</source>
        <translation>Frame</translation>
    </message>
    <message>
        <source>MCA Ch. : </source>
        <translation>MCA Ch. : </translation>
    </message>
    <message>
        <source>Cursor point : </source>
        <translation type="obsolete">カーソル位置: </translation>
    </message>
    <message>
        <source>Val. at Cursor : </source>
        <translation type="obsolete">カーソル位置の値 : </translation>
    </message>
    <message>
        <source>ROI start : </source>
        <translation type="obsolete">ROI始点</translation>
    </message>
    <message>
        <source>ROI end : </source>
        <translation type="obsolete">ROI終点: </translation>
    </message>
    <message>
        <source>Cur. [keV] : </source>
        <translation type="obsolete">カーソル [keV] : </translation>
    </message>
    <message>
        <source>Cur. [ch] : </source>
        <translation type="obsolete">カーソル [ch] : </translation>
    </message>
    <message>
        <source>Val. Cur : </source>
        <translation type="obsolete">値(カーソル位置) : </translation>
    </message>
    <message>
        <source>ROI s [keV] : </source>
        <translation type="obsolete">ROI始点[keV] : </translation>
    </message>
    <message>
        <source>ROI e [keV] : </source>
        <translation type="obsolete">ROI終点[keV] : </translation>
    </message>
    <message>
        <source>ROI value : </source>
        <translation type="obsolete">ROI積分値: </translation>
    </message>
    <message>
        <source>CPS in ROI : </source>
        <translation type="obsolete">ROI内[cps] : </translation>
    </message>
    <message>
        <source>Cursor</source>
        <translation>カーソル</translation>
    </message>
    <message>
        <source> Pos. [keV] : </source>
        <translation> 位置[keV] : </translation>
    </message>
    <message>
        <source> Pos. [ch] : </source>
        <translation> 位置[ch] : </translation>
    </message>
    <message>
        <source> Val. : </source>
        <translation> 値 : </translation>
    </message>
    <message>
        <source>ROI </source>
        <translation>ROI </translation>
    </message>
    <message>
        <source> Start [keV] : </source>
        <translation> 始点[keV] : </translation>
    </message>
    <message>
        <source> End [keV] : </source>
        <translation> 終点[keV] : </translation>
    </message>
    <message>
        <source> Count : </source>
        <translation> カウント : </translation>
    </message>
    <message>
        <source>   CPS : </source>
        <translation>   CPS :</translation>
    </message>
    <message>
        <source>Real Time : </source>
        <translation>実時間: </translation>
    </message>
    <message>
        <source>Live Time : </source>
        <translation>ライブ時間: </translation>
    </message>
    <message>
        <source>Dead Time : </source>
        <translation>デッドタイム: </translation>
    </message>
    <message>
        <source>Pop/Dock</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MUnits</name>
    <message>
        <source>Dwell time was set [%1] for [%2], though tried to be as [%3].</source>
        <translation>検出器[%2]の計測時間を[%3]に設定しようとしましたが、実際には[%1]に設定されました。</translation>
    </message>
    <message>
        <source>Warning on dwell time</source>
        <translation>計測時間に関する警告</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <source>MainWindow</source>
        <translation>XAFS測定</translation>
    </message>
    <message>
        <source>View 1</source>
        <translation>View1</translation>
    </message>
    <message>
        <source>View 2</source>
        <translation>View2</translation>
    </message>
    <message>
        <source>View 3</source>
        <translation>View3</translation>
    </message>
    <message>
        <source>View 4</source>
        <translation>View4</translation>
    </message>
    <message>
        <source>View5</source>
        <translation>View5</translation>
    </message>
    <message>
        <source>View6</source>
        <translation>View6</translation>
    </message>
    <message>
        <source>View7</source>
        <translation>View7</translation>
    </message>
    <message>
        <source>View8</source>
        <translation>View8</translation>
    </message>
    <message>
        <source>View9</source>
        <translation>View9</translation>
    </message>
    <message>
        <source>View10</source>
        <translation>View10</translation>
    </message>
    <message>
        <source>Select an Atom</source>
        <translation></translation>
    </message>
    <message>
        <source>Select a Target Atom</source>
        <translation></translation>
    </message>
    <message>
        <source>Periodic Table</source>
        <translation>周期表</translation>
    </message>
    <message>
        <source>Target</source>
        <translation>対象元素</translation>
    </message>
    <message>
        <source>Edge</source>
        <translation>対象殻</translation>
    </message>
    <message>
        <source>Select an absorption edge</source>
        <translation></translation>
    </message>
    <message>
        <source>  Edge</source>
        <translation>エッジ位置</translation>
    </message>
    <message>
        <source>KeV  </source>
        <translation>keV</translation>
    </message>
    <message>
        <source>Deg.</source>
        <translation>度</translation>
    </message>
    <message>
        <source>KeV</source>
        <translation type="obsolete">keV</translation>
    </message>
    <message>
        <source>Deg.  </source>
        <translation>度</translation>
    </message>
    <message>
        <source>10.0000</source>
        <translation></translation>
    </message>
    <message>
        <source>.</source>
        <translation></translation>
    </message>
    <message>
        <source>Cur. Pos.: </source>
        <translation>現在位置</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Usually the &amp;quot;Encorder&amp;quot; should be selected.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;通常は &amp;quot;エンコーダ&amp;quot;を選択&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>Encorder</source>
        <translation>エンコーダ</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;The &amp;quot;Calc from Pulse&amp;quot; should not be used for usual measurement.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;パルスモータのパルスから計算した角度。通常は&amp;quot;エンコーダ&amp;quot;を選択&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>Calc from Pulse</source>
        <translation>パルス換算</translation>
    </message>
    <message>
        <source>mA</source>
        <translation>mA</translation>
    </message>
    <message>
        <source>RIng Current</source>
        <translation>リング電流</translation>
    </message>
    <message>
        <source>Hide Tabs of Measurements and Settings</source>
        <translation>測定・條件設定のタブ部分を隠します</translation>
    </message>
    <message>
        <source>Hide</source>
        <translation type="obsolete">隠</translation>
    </message>
    <message>
        <source>Measurement</source>
        <translation>XAFS測定</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Normally the dwell-inputs indicate dwell-times for each measurement step,&lt;/p&gt;&lt;p&gt;but in the Q-XAFS mode, it indicates total measurement time.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>通常のXAFS測定では「計測時間」は１点の測定時間を、
Q-XAFSモードでは１スキャンに必要な時間を表します。</translation>
    </message>
    <message>
        <source>Mesure on Backward</source>
        <translation type="obsolete">戻りでも測定を行う</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Allow maximum speed, in the measurement region.&lt;/p&gt;&lt;p&gt;In some case, it takes longer time for the total scan process,&lt;/p&gt;&lt;p&gt;as it spend longer time to reach the top speed.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>最大速度になる設定を許します。
加減速にかかる時間があるため、場合によっては最大速度にするとトータルの測定時間は最短ではなくなることがあります。</translation>
    </message>
    <message>
        <source>Max Speed</source>
        <translation>最大速度</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Limit the top speed to realize the minimum scan time.&lt;/p&gt;&lt;p&gt;Optimize the total time including the time to run up to the top speed.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>最短の測定時間を達成するためにスキャンスピードの上限を制限する。
加速にかかる時間も含めたトータルの時間が最短になるように上限を設定します。</translation>
    </message>
    <message>
        <source>Min Time</source>
        <translation>最短時間</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Select an ion-chamber for I0.   The recommended gas is displayed in the following field.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>I0 の計測に使うイオンチャンバを選ぶと、推奨される混合ガスが次の欄に表示されます。</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Recommended gas for I0 chamber.&lt;/p&gt;&lt;p&gt;To use the recommended gas, you need to exchange it manually.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="obsolete">I0 チャンバに使用するお勧め混合ガス。
表示されたガスに自動的には切り替わらないので手動で切り替える必要があります。</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Select an ion-chamber for I1.   The recommended gas is displayed in the following field.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>I1 の計測に使うイオンチャンバを選ぶと、推奨される混合ガスが次の欄に表示されます。</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Recommended gas for I1 chamber.&lt;/p&gt;&lt;p&gt;To use the recommended gas, you need to exchange it manually.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="obsolete">I1 チャンバに使用するお勧め混合ガス。
表示されたガスに自動的には切り替わらないので手動で切り替える必要があります。</translation>
    </message>
    <message>
        <source>Range of the device selected to measure A1.</source>
        <translation>「その他1」を測定する検出器のレンジ</translation>
    </message>
    <message>
        <source>Range of the device selected to measure I0.</source>
        <translation>I0 を測定する検出器のレンジ</translation>
    </message>
    <message>
        <source>Range of the device selected to measure I1.</source>
        <translation>I1 を測定する検出器のレンジ</translation>
    </message>
    <message>
        <source>Range of the device selected to measure A2.</source>
        <translation>「その他2」を測定する検出器のレンジ</translation>
    </message>
    <message>
        <source>Use novel 1303 data format to record the measured data instead of the usual 9809 format.</source>
        <translation type="obsolete">従来の 9809 フォーマットの代わりに 1303 フォーマットを用いて測定結果を記録する。</translation>
    </message>
    <message>
        <source>Use 1303 Format</source>
        <translation type="obsolete">1303フォーマット使用</translation>
    </message>
    <message>
        <source>When 19ch SSD is used for the measurement, the MCA spectra for each channel is recorded at every measured points.</source>
        <translation type="obsolete">19ch SSDを使用する際、測定の各ステップでの MCA スペクトルを全て(全チャンネル、全ステップ)記録する。</translation>
    </message>
    <message>
        <source>Record MCA Spectra</source>
        <translation type="obsolete">MCAスペクトル記録</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;To measure near edge region precisely.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;XANES: ニアエッジ領域を詳細に測定&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>Std. XANES</source>
        <translation>標準XANES</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Both EXAFS and XAFNES regions are measured precisely at once.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;XAFS: XANES領域とEXAFS領域の両方を詳細に測定&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>Std. XAFS</source>
        <translation>標準XAFS</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>保存</translation>
    </message>
    <message>
        <source>Load</source>
        <translation>読込</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;To measure focusing on the &apos;EXAFS&apos; region. &lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;EXAFS: EXAFS領域に重点を置いた測定&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>Std. EXAFS</source>
        <translation>標準EXAFS</translation>
    </message>
    <message>
        <source>1</source>
        <translation>1</translation>
    </message>
    <message>
        <source>2</source>
        <translation>2</translation>
    </message>
    <message>
        <source>3</source>
        <translation>3</translation>
    </message>
    <message>
        <source>4</source>
        <translation>4</translation>
    </message>
    <message>
        <source>5</source>
        <translation>5</translation>
    </message>
    <message>
        <source>6</source>
        <translation>6</translation>
    </message>
    <message>
        <source>7</source>
        <translation>7</translation>
    </message>
    <message>
        <source>StartP</source>
        <translation>始点</translation>
    </message>
    <message>
        <source>Step</source>
        <translation>間隔</translation>
    </message>
    <message>
        <source>Steps</source>
        <translation>刻数</translation>
    </message>
    <message>
        <source>Blocks: </source>
        <translation>ブロック数</translation>
    </message>
    <message>
        <source>Dwell [s]</source>
        <translation>計測時間[秒]</translation>
    </message>
    <message>
        <source>Set all dwell times at once</source>
        <translation>全計測時間を一斉に設定</translation>
    </message>
    <message>
        <source>All: </source>
        <translation>All: </translation>
    </message>
    <message>
        <source>Data File Name</source>
        <translation>データファイル</translation>
    </message>
    <message>
        <source>...</source>
        <translation>選択</translation>
    </message>
    <message>
        <source>Comment</source>
        <translation>コメント</translation>
    </message>
    <message>
        <source>Detecotr Select</source>
        <translation type="obsolete">検出器選択</translation>
    </message>
    <message>
        <source>    I0</source>
        <translation>　I0</translation>
    </message>
    <message>
        <source>I</source>
        <translation type="obsolete">I</translation>
    </message>
    <message>
        <source>Aux. 1</source>
        <translation>その他 1</translation>
    </message>
    <message>
        <source>Aux. 2</source>
        <translation>その他 2</translation>
    </message>
    <message>
        <source>19ch SSD</source>
        <translation>19ch SSD</translation>
    </message>
    <message>
        <source>The change of the repitation No. is available, even after starting measurement.
Though, other parameters are fixed at the value on starting.</source>
        <translation>「繰返し回数」は測定開始後も変更可能(その他の測定に関わるパラメータは測定後に変更しても反映されない)</translation>
    </message>
    <message>
        <source>Repeat</source>
        <translation>繰返し</translation>
    </message>
    <message>
        <source>On Finish</source>
        <translation>終了時動作</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>開始</translation>
    </message>
    <message>
        <source>Pause</source>
        <translation>一時停止</translation>
    </message>
    <message>
        <source>Check Cond.</source>
        <translation>条件確認</translation>
    </message>
    <message>
        <source>Total points which will be measured</source>
        <translation>総測定点数</translation>
    </message>
    <message>
        <source>Total points which will be measured
</source>
        <translation>総測定点数</translation>
    </message>
    <message>
        <source>Points:</source>
        <translation>点数:</translation>
    </message>
    <message>
        <source>Total measurement time</source>
        <translation>予想測定時間</translation>
    </message>
    <message>
        <source>Time:</source>
        <translation>時間:</translation>
    </message>
    <message>
        <source>Meas. Bacground Time.[s] </source>
        <translation type="obsolete">バックグラウンド計測時間 [s] </translation>
    </message>
    <message>
        <source>Meas. Background</source>
        <translation>バックグラウンド計測</translation>
    </message>
    <message>
        <source>Meas. before Scan</source>
        <translation type="obsolete">測定前にバックグラウンド計測</translation>
    </message>
    <message>
        <source>Show BGs</source>
        <translation>計測値表示</translation>
    </message>
    <message>
        <source>Save the block definitions.</source>
        <translation>測定ブロック設定のセーブ</translation>
    </message>
    <message>
        <source>Load the saved block definitions.</source>
        <translation>測定ブロック設定の読込</translation>
    </message>
    <message>
        <source>Start Time</source>
        <translation>開始時間</translation>
    </message>
    <message>
        <source>Now</source>
        <translation>現在時間</translation>
    </message>
    <message>
        <source>End TIme</source>
        <translation type="obsolete">終了時間</translation>
    </message>
    <message>
        <source>HSpeed</source>
        <translation type="obsolete">スキャンスピード(pps)</translation>
    </message>
    <message>
        <source>0000</source>
        <translation>0000</translation>
    </message>
    <message>
        <source>Limited Display</source>
        <translation>表示制限</translation>
    </message>
    <message>
        <source>Last </source>
        <translation>直近</translation>
    </message>
    <message>
        <source> Lines</source>
        <translation>本 表示</translation>
    </message>
    <message>
        <source>Q-XAFS mode</source>
        <translation>Q-XAFS モード</translation>
    </message>
    <message>
        <source>Down</source>
        <translation>下流</translation>
    </message>
    <message>
        <source>Stream</source>
        <translation> </translation>
    </message>
    <message>
        <source>Up</source>
        <translation>上流</translation>
    </message>
    <message>
        <source>Show Energy</source>
        <translation>エネルギー表示</translation>
    </message>
    <message>
        <source>ROI changeable even while the XAFS measurements</source>
        <translation type="obsolete">XAFS測定中もROIの変更を可能にする</translation>
    </message>
    <message>
        <source>21</source>
        <translation>21</translation>
    </message>
    <message>
        <source>23</source>
        <translation>23</translation>
    </message>
    <message>
        <source>02</source>
        <translation>02</translation>
    </message>
    <message>
        <source>22</source>
        <translation>22</translation>
    </message>
    <message>
        <source>19</source>
        <translation>19</translation>
    </message>
    <message>
        <source>03</source>
        <translation>03</translation>
    </message>
    <message>
        <source>09</source>
        <translation>09</translation>
    </message>
    <message>
        <source>08</source>
        <translation>08</translation>
    </message>
    <message>
        <source>07</source>
        <translation>07</translation>
    </message>
    <message>
        <source>06</source>
        <translation>06</translation>
    </message>
    <message>
        <source>05</source>
        <translation>05</translation>
    </message>
    <message>
        <source>24</source>
        <translation>24</translation>
    </message>
    <message>
        <source>01</source>
        <translation>01</translation>
    </message>
    <message>
        <source>04</source>
        <translation>04</translation>
    </message>
    <message>
        <source>20</source>
        <translation>20</translation>
    </message>
    <message>
        <source>Set up Conditions</source>
        <translation>条件設定</translation>
    </message>
    <message>
        <source>Go</source>
        <translation>移動</translation>
    </message>
    <message>
        <source>Move To (2)</source>
        <translation>移動先 (2)</translation>
    </message>
    <message>
        <source>Move To (3)</source>
        <translation>移動先 (3)</translation>
    </message>
    <message>
        <source>Move To (4)</source>
        <translation>移動先 (4)</translation>
    </message>
    <message>
        <source> Speed </source>
        <translation>スピード</translation>
    </message>
    <message>
        <source>H</source>
        <translation>H</translation>
    </message>
    <message>
        <source>M</source>
        <translation>M</translation>
    </message>
    <message>
        <source>L</source>
        <translation>L</translation>
    </message>
    <message>
        <source>Move Monochro</source>
        <translation type="obsolete">分光器回転</translation>
    </message>
    <message>
        <source>Move To (1)</source>
        <translation>移動先 (1)</translation>
    </message>
    <message>
        <source>0.1</source>
        <translation>0.1</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>計測値</translation>
    </message>
    <message>
        <source>Dwell T.</source>
        <translation>計測時間</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>選択</translation>
    </message>
    <message>
        <source>Mon. Start</source>
        <translation>モニタ開始</translation>
    </message>
    <message>
        <source>0.000</source>
        <translation>0.000</translation>
    </message>
    <message>
        <source>Monitor Sensors</source>
        <translation>検出器モニタ</translation>
    </message>
    <message>
        <source> Dwell [s]:</source>
        <translation>計測時間[s]:</translation>
    </message>
    <message>
        <source>Scan</source>
        <translation>スキャン開始</translation>
    </message>
    <message>
        <source>StartP:</source>
        <translation>始点:</translation>
    </message>
    <message>
        <source>Abs</source>
        <translation type="obsolete">絶対</translation>
    </message>
    <message>
        <source>EndP:</source>
        <translation>終点:</translation>
    </message>
    <message>
        <source>Step:</source>
        <translation>間隔:</translation>
    </message>
    <message>
        <source>Rel</source>
        <translation type="obsolete">相対</translation>
    </message>
    <message>
        <source>Save:</source>
        <translation>保存ファイル:</translation>
    </message>
    <message>
        <source>puls</source>
        <translation>puls</translation>
    </message>
    <message>
        <source>Move To</source>
        <translation>移動先</translation>
    </message>
    <message>
        <source>unit</source>
        <translation>単位</translation>
    </message>
    <message>
        <source>Position</source>
        <translation>現在位置</translation>
    </message>
    <message>
        <source>Monitor:</source>
        <translation>検出器:</translation>
    </message>
    <message>
        <source>0</source>
        <translation>0</translation>
    </message>
    <message>
        <source>Scan/Move a Motor</source>
        <translation>移動/スキャン</translation>
    </message>
    <message>
        <source>KeV   </source>
        <translation>keV</translation>
    </message>
    <message>
        <source>Measure before Scan</source>
        <translation type="obsolete">スキャン前に計測</translation>
    </message>
    <message>
        <source>Measure Background</source>
        <translation>バックグラウンド計測</translation>
    </message>
    <message>
        <source>Use Measured</source>
        <translation>計測済の値を使用</translation>
    </message>
    <message>
        <source>Meas. Bacground Time.[s]</source>
        <translation type="obsolete">バックグラウンド計測時間[秒]</translation>
    </message>
    <message>
        <source>By clicking the &apos;A. Scale&apos; (Auto Scale) button at left-bottom corner, the auto-scale mode is toggled on and off.</source>
        <translation type="obsolete">左下の「A. Scale」 (Auto Scale)と書かれたボタンを押すことで、オートスケールモードをオン・オフできます。</translation>
    </message>
    <message>
        <source>When it is displayed as &quot;Shutter Close!&quot;, check the shutter is closed and push the button.
When it is displayed as &quot;Shutter Open!, check the shutter is opend and push the button.</source>
        <translation>「シャッター CLOSE 確認」と表示されたときは、シャッターが閉まっていることを確認してボタンを押して下さい。
「シャッター OPEN 確認」と表示されたときは、シャッターが開いていることを確認してボタンを押して下さい。</translation>
    </message>
    <message>
        <source>Back ground</source>
        <translation type="obsolete">バックグラウンド</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;When the &apos;Normalize&apos; is checked, the value of the detector selected as &apos;Monitor&apos; is normalized by the value of the detecter selected in the following selection-box.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;「規格化」がチェックされていると、「計測器:」で選択した計測器の値を「規格化」で選択した計測器の値で規格化します。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>規格化</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;The value of the selected detecter is always (with or without check the &apos;Normalize&apos;) recorded with the value of the detecter selected as &apos;Monitor&apos; for the scan.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>「規格化」で選択した測定器の値は、「規格化」をチェックしていてもしていなくても、主たる計測対象の「計測器：」で選択した機器の測定値と共に常に測定ファイルに記録されます。</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Select measurement renge of the sensor.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;測定器の測定レンジを選択して下さい。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>Select Range</source>
        <translation>レンジ選択</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Get the selected range of the selected sensor.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>指定した計測器の現在のレンジ取得</translation>
    </message>
    <message>
        <source>Get Range</source>
        <translation>レンジ取得</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Get the selected ranges of all of the sensors.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;レンジ選択のある全計測器の現在のレンジを取得する。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>Get All Range</source>
        <translation>全レンジ取得</translation>
    </message>
    <message>
        <source>Set up SSD</source>
        <translation>SSD設定</translation>
    </message>
    <message>
        <source>At Cursor</source>
        <translation>カーソル点</translation>
    </message>
    <message>
        <source>Log  </source>
        <translation>Log </translation>
    </message>
    <message>
        <source>eV  Gain</source>
        <translation type="obsolete">eV  ゲイン</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>クリア</translation>
    </message>
    <message>
        <source>Live</source>
        <translation>ライブ</translation>
    </message>
    <message>
        <source>Real</source>
        <translation>リアル</translation>
    </message>
    <message>
        <source>Preset:</source>
        <translation type="obsolete">プリセット</translation>
    </message>
    <message>
        <source>  ROI end</source>
        <translation type="obsolete">積分終点</translation>
    </message>
    <message>
        <source>  In ROI</source>
        <translation type="obsolete">積分値</translation>
    </message>
    <message>
        <source>u-sec</source>
        <translation>μs</translation>
    </message>
    <message>
        <source>eV</source>
        <translation>eV</translation>
    </message>
    <message>
        <source>mV/keV</source>
        <translation>mV/keV</translation>
    </message>
    <message>
        <source>ROI  Start</source>
        <translation>積分始点</translation>
    </message>
    <message>
        <source>Peak. T.</source>
        <translation>ピーキング時間</translation>
    </message>
    <message>
        <source>eV  Carib.</source>
        <translation type="obsolete">eV  較正</translation>
    </message>
    <message>
        <source>sec</source>
        <translation>秒</translation>
    </message>
    <message>
        <source>Thresh.</source>
        <translation>閾値</translation>
    </message>
    <message>
        <source>Dyn. Range</source>
        <translation>ダイナミックレンジ</translation>
    </message>
    <message>
        <source>SSD No.</source>
        <translation>SSD チャンネル</translation>
    </message>
    <message>
        <source>8</source>
        <translation>8</translation>
    </message>
    <message>
        <source>9</source>
        <translation>9</translation>
    </message>
    <message>
        <source>SetUp BLKs</source>
        <translation>測定ブロック設定</translation>
    </message>
    <message>
        <source>Select Detectors</source>
        <translation>検出器選択</translation>
    </message>
    <message>
        <source>Start Meas.</source>
        <translation>測定開始</translation>
    </message>
    <message>
        <source>Set Up a SSD Channel</source>
        <translation>SSDの各チャンネルの設定</translation>
    </message>
    <message>
        <source>10</source>
        <translation>10</translation>
    </message>
    <message>
        <source>11</source>
        <translation>11</translation>
    </message>
    <message>
        <source>12</source>
        <translation>12</translation>
    </message>
    <message>
        <source>13</source>
        <translation>13</translation>
    </message>
    <message>
        <source>14</source>
        <translation>14</translation>
    </message>
    <message>
        <source>15</source>
        <translation>15</translation>
    </message>
    <message>
        <source>16</source>
        <translation>16</translation>
    </message>
    <message>
        <source>17</source>
        <translation>17</translation>
    </message>
    <message>
        <source>18</source>
        <translation>18</translation>
    </message>
    <message>
        <source>Select a SSD ch (MCA spectrum)</source>
        <translation>SSD選択(MCAスペクトル)</translation>
    </message>
    <message>
        <source>Select SSD channels (XAFS meas.)</source>
        <translation>複数SSD選択(積算/本測定対象)</translation>
    </message>
    <message>
        <source>I                 </source>
        <translation></translation>
    </message>
    <message>
        <source>Set Offset</source>
        <translation>設定</translation>
    </message>
    <message>
        <source>Check/Set Offset(Dark)</source>
        <translation>バックグラウンド確認／設定</translation>
    </message>
    <message>
        <source>Auto Rang</source>
        <translation type="obsolete">オートレンジ</translation>
    </message>
    <message>
        <source>Move a motor or scan by moving a motor while monitoring a sensor.</source>
        <translation>駆動軸を動かす、もしくは軸を動かしながらスキャンを行う。</translation>
    </message>
    <message>
        <source>Move the monochromator where you want.</source>
        <translation>分光器の角度設定</translation>
    </message>
    <message>
        <source>Select an unit for all.</source>
        <translation>全部の単位を一度に設定します。</translation>
    </message>
    <message>
        <source>When the sensor can be used in auto-range mode, the check button is active.</source>
        <translation>選択した検出器でオートレンジモードが使える場合、このチェックボタンが有効になります。</translation>
    </message>
    <message>
        <source>Make info File</source>
        <translation type="obsolete">Info ファイル作成</translation>
    </message>
    <message>
        <source>Current Pos.</source>
        <translation>現在位置</translation>
    </message>
    <message>
        <source>Rpt.</source>
        <translation>回目</translation>
    </message>
    <message>
        <source>Point</source>
        <translation>点目</translation>
    </message>
    <message>
        <source>Record the data during the monitor.</source>
        <translation>モニータ時のデータを記録します。(モニタ開始前にチェック)</translation>
    </message>
    <message>
        <source>Record</source>
        <translation>記録</translation>
    </message>
    <message>
        <source>Save the monitored data after the monitoring.</source>
        <translation>(モニタ後)モニタしたデータを記録します。</translation>
    </message>
    <message>
        <source>Disp. Elm. Names</source>
        <translation>元素名表示</translation>
    </message>
    <message>
        <source>Select Elms.</source>
        <translation>表示元素選択</translation>
    </message>
    <message>
        <source>Show Always</source>
        <translation>選択した元素を常に表示</translation>
    </message>
    <message>
        <source>eV        Gain</source>
        <translation>eV       ゲイン</translation>
    </message>
    <message>
        <source>      ROI end</source>
        <translation>     積分終点</translation>
    </message>
    <message>
        <source>         In ROI</source>
        <translation>       積分値</translation>
    </message>
    <message>
        <source>eV       Carib.</source>
        <translation type="obsolete">eV        較正.</translation>
    </message>
    <message>
        <source>Set All</source>
        <translation>Set All</translation>
    </message>
    <message>
        <source>Read Data</source>
        <translation>データ読込</translation>
    </message>
    <message>
        <source>Close View</source>
        <translation>View を閉じる</translation>
    </message>
    <message>
        <source>Log / Record</source>
        <translation>ログ/記録</translation>
    </message>
    <message>
        <source>Log File Name:</source>
        <translation>ログファイル名</translation>
    </message>
    <message>
        <source>New</source>
        <translation>選択</translation>
    </message>
    <message>
        <source>Add Log Comment:</source>
        <translation>ログコメント追加</translation>
    </message>
    <message>
        <source>Stat/Config</source>
        <translation>状態/設定</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="obsolete">ファイル(&amp;F)</translation>
    </message>
    <message>
        <source>&amp;Config</source>
        <translation type="obsolete">設定(&amp;C)</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>終了(&amp;Q)</translation>
    </message>
    <message>
        <source>Japanese</source>
        <translation>日本語</translation>
    </message>
    <message>
        <source>English</source>
        <translation>English</translation>
    </message>
    <message>
        <source>Set &amp;Lattice Const. Monochro Crystal</source>
        <translation>結晶・格子定数設定(&amp;L)</translation>
    </message>
    <message>
        <source>Set &amp;Stars Server Address</source>
        <translation>Starsサーバ設定(&amp;S)</translation>
    </message>
    <message>
        <source>Setup: %1 : Reached at %2</source>
        <translation>設定: %1 : %2 に到達</translation>
    </message>
    <message>
        <source>The name is new, but the data is old.</source>
        <translation>ファイル名は新規に選択されたものですが、データはすでに記録済みのデータです。</translation>
    </message>
    <message>
        <source>The data is new, but the name is old.</source>
        <translation>データは新しく測定されたものですが、ファイル名は古いままです。</translation>
    </message>
    <message>
        <source>The data and the name is new, but not saved.</source>
        <translation>データもファイル名も新しいものですが保存されていません。</translation>
    </message>
    <message>
        <source>Mono: %1 (%2 A)</source>
        <translation>分光結晶: %1 (%2 A)</translation>
    </message>
    <message>
        <source>Encorder is set from %1 to %2</source>
        <translation>エンコーダの値を %1 から %2 にセットしました</translation>
    </message>
    <message>
        <source>Current Position [%1] deg [%2] keV</source>
        <translation>現在位置 [%1] deg [%2] keV</translation>
    </message>
    <message>
        <source>Current Position [%1] keV</source>
        <translation type="obsolete">分光器現在位置 [%1] keV</translation>
    </message>
    <message>
        <source>No Scree is available!</source>
        <translation>使用可能な描画領域(View)がありません</translation>
    </message>
    <message>
        <source>Start Measurement!</source>
        <translation>測定開始</translation>
    </message>
    <message>
        <source>Writing Header.</source>
        <translation>ヘッダ記録</translation>
    </message>
    <message>
        <source>Meas: Repeat %1</source>
        <translation>測定： %1回目</translation>
    </message>
    <message>
        <source>The Measurement has Finished</source>
        <translation>測定終了</translation>
    </message>
    <message>
        <source>Meas: Finished</source>
        <translation>測定: 終了</translation>
    </message>
    <message>
        <source>mu</source>
        <translation>μt</translation>
    </message>
    <message>
        <source>Absorption edge of the atom is out of range.</source>
        <translation>対象原子の吸収端が測定可能範囲外です。</translation>
    </message>
    <message>
        <source>Return</source>
        <translation>原位置復帰</translation>
    </message>
    <message>
        <source>Stay</source>
        <translation>その場に停止</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>キャンセル</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>&lt;h1&gt;&lt;center&gt;Stop ?&lt;/center&gt;&lt;/h1&gt;</source>
        <translation>終了?</translation>
    </message>
    <message>
        <source>Stop or Continue</source>
        <translation>停止／再開</translation>
    </message>
    <message>
        <source>Over Write ?</source>
        <translation>上書きしますか?</translation>
    </message>
    <message>
        <source>Have you seleced ?</source>
        <translation>選択済み？</translation>
    </message>
    <message>
        <source>Points: </source>
        <translation>点数: </translation>
    </message>
    <message>
        <source>Time: </source>
        <translation>時間: </translation>
    </message>
    <message>
        <source>I0</source>
        <translation>I0</translation>
    </message>
    <message>
        <source>mu(E)</source>
        <translation>μ(E)</translation>
    </message>
    <message>
        <source>Energy</source>
        <translation></translation>
    </message>
    <message>
        <source>[keV]</source>
        <translation>[keV]</translation>
    </message>
    <message>
        <source>Monochro is moving!</source>
        <translation>分光器回転中</translation>
    </message>
    <message>
        <source>Meas cannot Start : (%1) is disabled</source>
        <translation>(%1) が使用できないため測定を開始できません。</translation>
    </message>
    <message>
        <source>Meas cannot Start : in MCA measurement</source>
        <translation>MCA 測定中のため XAFS 測定を開始できません</translation>
    </message>
    <message>
        <source>Measured points are too many.  It should be less than 9990 in QXAFS mode.</source>
        <translation>測定点数が多過ぎます。QXAFSモードでの上限は9900点です。</translation>
    </message>
    <message>
        <source>Measured points are too many.  It should be less than 9990.</source>
        <translation type="obsolete">測定繰返し回数が多過ぎます。回数は 9900 以下にして下さい。</translation>
    </message>
    <message>
        <source>I1 must be selected for QXAFS</source>
        <translation>Q-XAFS では、I1 は必ず選択する必要があります。</translation>
    </message>
    <message>
        <source>19ch SSD can not be used for QXAFS</source>
        <translation>Q-XAFS では 19素子SSDは使えません。</translation>
    </message>
    <message>
        <source>Aux1 and 2 can not be used for QXAFS</source>
        <translation>Q-XAFS では Aux1 および 2 は使用できません。</translation>
    </message>
    <message>
        <source>Selected I0 Sensor can not be used for QXAFS</source>
        <translation>I0としえて選択した検出器は Q-XAFS には使用できません。</translation>
    </message>
    <message>
        <source>Selected I1 Sensor can not be used for QXAFS</source>
        <translation>I1としえて選択した検出器は Q-XAFS には使用できません。</translation>
    </message>
    <message>
        <source>Measured points are too many.    It should be less than 2000 in normal XAFS mode.</source>
        <translation>測定点数が多過ぎます。通常XAFSモードでは測定点数は最大2000点です。</translation>
    </message>
    <message>
        <source>I1</source>
        <translation>I1</translation>
    </message>
    <message>
        <source>The sensor [%1] can not use for the QXafs.</source>
        <translation>[%1]はQXafsには使えません。</translation>
    </message>
    <message>
        <source>The sensor [%1] can not use for the Normal Xafs.</source>
        <translation>[%1]は通常のXAFS測定には使えません(QXAFS用です)。</translation>
    </message>
    <message>
        <source>Meas: Start %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</source>
        <translation>測定開始 %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</translation>
    </message>
    <message>
        <source>Meas: Break %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</source>
        <translation>測定中断 %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</translation>
    </message>
    <message>
        <source>Meas: Stopped %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</source>
        <translation>測定中断 : %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</translation>
    </message>
    <message>
        <source>Meas: Pausing %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</source>
        <translation>測定中断中 %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</translation>
    </message>
    <message>
        <source>Meas: Resume %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</source>
        <translation>測定再開 %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</translation>
    </message>
    <message>
        <source>Meas: Pause %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</source>
        <translation>測定中断 %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</translation>
    </message>
    <message>
        <source>BG Measuring</source>
        <translation>BG 計測中</translation>
    </message>
    <message>
        <source>Already Measuring Dark!</source>
        <translation>バックグラウンド計測中!</translation>
    </message>
    <message>
        <source>Measurement is going on!</source>
        <translation>測定中!</translation>
    </message>
    <message>
        <source>MCA measurement is going on</source>
        <translation>MCA測定中</translation>
    </message>
    <message>
        <source>Scan cannot Start : (%1) is disabled</source>
        <translation>%1 が disable なので、測定開始できません。</translation>
    </message>
    <message>
        <source>Close Shutter!!</source>
        <translation>シャッター CLOSE 確認</translation>
    </message>
    <message>
        <source>Make sure that shutte is closed.  Then push the &apos;red&apos; button.</source>
        <translation>シャッターが閉まっていることを確認し、赤くなっている「シャッター CLOSE 確認」ボタンを押して下さい。</translation>
    </message>
    <message>
        <source>Invalid dwell time [%1] was set for [%2].However, the background was set as if the time was set at 1sec.</source>
        <translation>計測器[%2]に指定された計測時間[%1]が正しくありません。1秒に設定して計測します。</translation>
    </message>
    <message>
        <source>Open Shutter!!</source>
        <translation>シャッター OPEN 確認</translation>
    </message>
    <message>
        <source>Make sure that shutte is opened.  Then push the &apos;red&apos; button.</source>
        <translation>シャッタがーが開いていることを確認し、赤くなっている「シャッター OPEN 確認」ボタンを押して下さい。</translation>
    </message>
    <message>
        <source>Invalid block data.</source>
        <translation>測定ブロックの指定が誤っています。</translation>
    </message>
    <message>
        <source>Data File is not Selected!</source>
        <translation>データファイルが選択されていません。</translation>
    </message>
    <message>
        <source>Detectors are not selected properly!</source>
        <translation type="obsolete">測定のための検出器の指定が正しくありません。</translation>
    </message>
    <message>
        <source>Selected sensors [%1] and [%2] are conflicting.</source>
        <translation>選択した[%1]と[%2]は同時に使えません。</translation>
    </message>
    <message>
        <source>The Sensor(s)%1 should be range selected.
Have you selected the range in &apos;Setup Condition&apos;</source>
        <translation type="obsolete">選択した計測器 %1 について、「設定」の画面でレンジを設定しましたか？</translation>
    </message>
    <message>
        <source>File [%1] Over Write ?</source>
        <translation>ファイル [%1] を上書きして宜しいですか？</translation>
    </message>
    <message>
        <source>&lt;h1&gt;&lt;center&gt;File [%1] Over Write ?&lt;/center&gt;&lt;/h1&gt;</source>
        <translation type="obsolete">&lt;h1&gt;&lt;center&gt;ファイル [%1] に上書きして宜しいですか?&lt;/center&gt;&lt;/h1&gt;</translation>
    </message>
    <message>
        <source>Meas: Start (%1 keV)</source>
        <translation type="obsolete">測定: 開始(%1 keV)</translation>
    </message>
    <message>
        <source>Stop</source>
        <translation>中止</translation>
    </message>
    <message>
        <source>Meas: Break (%1 keV)</source>
        <translation type="obsolete">測定: 中断(%1 keV)</translation>
    </message>
    <message>
        <source>Resume</source>
        <translation>再開</translation>
    </message>
    <message>
        <source>Meas: Stopped (%1 keV)</source>
        <translation type="obsolete">測定: 停止 (%1 keV)</translation>
    </message>
    <message>
        <source>The Measurement is Stopped</source>
        <translation>測定が中止されました。</translation>
    </message>
    <message>
        <source>Meas: Pausing (%1 keV)</source>
        <translation type="obsolete">測定: ポーズ (%1 keV)</translation>
    </message>
    <message>
        <source>Measu: Resume (%1 keV)</source>
        <translation type="obsolete">測定: 再開 (%1 keV)</translation>
    </message>
    <message>
        <source>Meas: Pause (%1 keV)</source>
        <translation type="obsolete">測定: ポーズ (%1 keV)</translation>
    </message>
    <message>
        <source>Meas: Resume (%1 keV)</source>
        <translation type="obsolete">測定: 再開 (%1 keV)</translation>
    </message>
    <message>
        <source>Active</source>
        <translation>アクティブ</translation>
    </message>
    <message>
        <source>MCA data is not valid</source>
        <translation>正しいMCAデータがありません。</translation>
    </message>
    <message>
        <source>Save file name is not selected</source>
        <translation>ファイル名が選択されていません。</translation>
    </message>
    <message>
        <source>The file [%1] can not open to record the data</source>
        <translation>記録のためのファイル [%1]が開けません。</translation>
    </message>
    <message>
        <source>Inactive</source>
        <translation>インアクティブ</translation>
    </message>
    <message>
        <source>ROI cannot change while the XAFS measurements</source>
        <translation>XAFS測定中はROIを変更できません</translation>
    </message>
    <message>
        <source>Scan cannot Start : SSD is disabled</source>
        <translation>SSD が disable なので測定が開始できません。</translation>
    </message>
    <message>
        <source>Scan data is not valid</source>
        <translation>正しいスキャンデータがありません。</translation>
    </message>
    <message>
        <source>Current Position of [%1] : [%2] %3</source>
        <translation>[%1]の現在位置 : [%2] %3</translation>
    </message>
    <message>
        <source>Setup: %1 : GoTo %2 : Speed %3</source>
        <translation>設定: %1 をスピード %3 で %2 に移動</translation>
    </message>
    <message>
        <source>Setup: %1 : Stopped at %2</source>
        <translation>設定: %1 を %2 で停止</translation>
    </message>
    <message>
        <source>No drawing screen is available</source>
        <translation>使用可能な描画領域(View)がありません。</translation>
    </message>
    <message>
        <source>The Sensor (%1) can use only in QXafs mode.</source>
        <translation>[%1]はQXAFS専用です。</translation>
    </message>
    <message>
        <source>Error: Scan Step is 0.</source>
        <translation>エラー: スキャンステップが 0 です。</translation>
    </message>
    <message>
        <source>Scan Start (%1 %2)</source>
        <translation>スキャン: 開始 (%1, %2)</translation>
    </message>
    <message>
        <source>No drawing area is avairable</source>
        <translation>使用可能な描画領域(View)がありません。</translation>
    </message>
    <message>
        <source>No Record file is selected</source>
        <translation>記録ファイルが選択されていません。</translation>
    </message>
    <message>
        <source>The Sensor [%1] can use only in QXafs mode.</source>
        <translation>[%1]はQXAFS専用です。</translation>
    </message>
    <message>
        <source>	%1[%2]</source>
        <translation></translation>
    </message>
    <message>
        <source>Monitor data is not valid</source>
        <translation>有効なデータがありません。</translation>
    </message>
    <message>
        <source>File [%1] is selected as XafsM2 definition file.</source>
        <translation>ファイル[%1]が XafsM2 の定義ファイルに選択されました。</translation>
    </message>
    <message>
        <source>FilePath = [%1]</source>
        <translation>FilePath = [%1]</translation>
    </message>
    <message>
        <source>The file [%1] is not. Use default parameters.</source>
        <translation>The file [%1] is not. Use default parameters.</translation>
    </message>
    <message>
        <source>Error cannot open [%1].</source>
        <translation>Error cannot open [%1].</translation>
    </message>
    <message>
        <source>::Undefined Unit type [%1]</source>
        <translation>::Undefined Unit type [%1]</translation>
    </message>
    <message>
        <source>Undefined Key word [%1]</source>
        <translation>Undefined Key word [%1]</translation>
    </message>
    <message>
        <source>UIDs [%1](at line %2) and [%3](at line %4) are duplicated.</source>
        <translation></translation>
    </message>
    <message>
        <source>1st one is : Type[%1] Identifier[%2] Driver[%3] Node[%4]</source>
        <translation></translation>
    </message>
    <message>
        <source>2nd one is : Type[%1] Identifier[%2] Driver[%3] Node[%4]</source>
        <translation></translation>
    </message>
    <message>
        <source>Going to initial position.</source>
        <translation>開始点に移動中</translation>
    </message>
    <message>
        <source>The Scan has Finished</source>
        <translation>スキャン終了</translation>
    </message>
    <message>
        <source>Scan Finished</source>
        <translation>スキャン終了</translation>
    </message>
    <message>
        <source>Cannot open Log File [%1]
</source>
        <translation>ログファイル[%1]が開けません。</translation>
    </message>
    <message>
        <source>Cycle: </source>
        <translation>一周期:</translation>
    </message>
    <message>
        <source>Total: </source>
        <translation>総時間:</translation>
    </message>
    <message>
        <source>Selected Steps were too many!  It was changed to be %1</source>
        <translation type="obsolete">選択されたステップ数は多過ぎます。%1 に変更されました。</translation>
    </message>
    <message>
        <source>Selected Points were too many!  It was changed to be %1</source>
        <translation>指定された測定点数が多過ぎます。点数を%1に変更しました。</translation>
    </message>
    <message>
        <source>Start QXAFS Measurement!</source>
        <translation>Q-XAFS 測定開始</translation>
    </message>
    <message>
        <source>Fwd</source>
        <translation>往路</translation>
    </message>
    <message>
        <source>Bwd</source>
        <translation>復路</translation>
    </message>
    <message>
        <source>Meas: QXafs Finished</source>
        <translation>Q-XAFS 測定終了</translation>
    </message>
    <message>
        <source>Auto mode</source>
        <translation>自動試料交換</translation>
    </message>
    <message>
        <source>Please put the numder to be scanned (eg. 1,2,3 or 3-6)</source>
        <translation type="obsolete">自動交換して測定する試料番号を入力して下さい。
(例 1,3,5 : この3つをこの順に測定します。例 5, 1-4 : 5を測定した後、1〜4を順に測定します)</translation>
    </message>
    <message>
        <source></source>
        <translation></translation>
    </message>
    <message>
        <source>Auto mode parameters are not set.</source>
        <translation>自動交換する試料を選択して下さい。</translation>
    </message>
    <message>
        <source>Auto mode cannot be used with QXAFS mode</source>
        <translation type="obsolete">QXAFSモードでは試料の自動交換は行えません。</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Please put the numder to be scanned (eg. 1,2,3 or 3-6) &lt;img src=&quot;:/Changer.png&quot;/&gt;&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;自動交換して測定する試料番号を入力して下さい。&lt;p&gt;(例 1,3,5 : この3つをこの順に測定します。例 5, 1-4 : 5を測定した後、1〜4を順に測定します)&lt;p&gt; &lt;img src=&quot;:/Changer.png&quot;/&gt;&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>バックグランド</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Select a sample holder of which center is at the beam position.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;現在ビームがその中心を通っている試料ホルダ番号を選んで下さい&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Let the center of the selected No. of the sample holder is at the beam position.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;ビームが選択した番号の試料ホルダ中心を通っていると認識し、原点を再定義します。( 5番の中心が(0,0)になるようにします。)&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>Set C.C.</source>
        <translation>現位置セット</translation>
    </message>
    <message>
        <source>The position to go [%1]eV is out of range. [%2]-[%3]eV</source>
        <translation>移動しようとしているエネルギー[%1]eVは可動範囲外です。可動範囲は [%2]-[%3]eV です。</translation>
    </message>
    <message>
        <source>Auto mode parameters are illigal.</source>
        <translation>試料自動交換の指定(測定順序等の書き方)が間違っています。</translation>
    </message>
    <message>
        <source>Please choose &quot;Use Measured&quot;</source>
        <translation>連続測定が中断されないよう、バックグラウンドの計測に関して「計測済みの値を使用」を選択して下さい。</translation>
    </message>
    <message>
        <source>The block definitin [%1]eV is out of range. [%2]-[%3]eV</source>
        <translation>ブロックの定義にある [%1]eV は可動範囲外です。 可動範囲は [%2]-[%3]eV です。</translation>
    </message>
    <message>
        <source>A.M. Enable</source>
        <translation>使用</translation>
    </message>
    <message>
        <source>  Peak List: </source>
        <translation>ピークリスト</translation>
    </message>
    <message>
        <source>Calibrate as: </source>
        <translation>較正:</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Typically, it should be 0.5.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;通常、0.5。大きくするとピークを見つけにくくなり、小さくするとノイズなども誤ってピークと判断しやすくなる。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>[keV]   Sensitivity </source>
        <translation type="obsolete">[keV]   感度 </translation>
    </message>
    <message>
        <source>0.5</source>
        <translation>0.5</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Show differential curve of the MCA spectrum.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;微分スペクトルを表示するかどうかの選択&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>Show Diff.</source>
        <translation>微分表示</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Last X [Lines] will be displayed, and for the lines older than the &apos;X&apos;, lines measured at every Y [periods] will be displayed. &lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;直近に測定された X[本] のスペクトルが表示されます。それより前に測定されたスペクトルは Y[間隔]本毎に表示されます。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>Period</source>
        <translation>間隔</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;When the button is checked, the file extentions will be numbers for 2nd and later files, like as, .001, .002, .003,...   When the button is not checked, the series number will be add at the end of file-name-body and the file extentions will be always .dat, like as, test-001.dat, test-002.dat,...&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;このボタンがチェックされていると、繰り返し測定の2回目以降のファイル名は拡張子が .001, .002, .003,... の様になります。&lt;/p&gt;
&lt;p&gt;チェックが外れていると、拡張子は常に .dat になり、基幹ファイル名の末尾に -001, -002, 等が追加され、例えば test-001.dat, test-002.dat,... の様になります。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>.dat/.num</source>
        <translation>.dat/.num</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Number of items which will be measured.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;自動測定を行う点数&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>Items</source>
        <translation>点数</translation>
    </message>
    <message>
        <source>  Preset:</source>
        <translation>  プリセット: </translation>
    </message>
    <message>
        <source>Integration</source>
        <translation>積算</translation>
    </message>
    <message>
        <source>Disp. Elm.</source>
        <translation>元素表示</translation>
    </message>
    <message>
        <source>[keV] </source>
        <translation>[keV]</translation>
    </message>
    <message>
        <source>Peak Search</source>
        <translation>ピークサーチ</translation>
    </message>
    <message>
        <source>Sensitivity: </source>
        <translation>感度: </translation>
    </message>
    <message>
        <source>Parameters</source>
        <translation>パラメータ</translation>
    </message>
    <message>
        <source>Intervals:   at Start</source>
        <translation type="obsolete">休止:  始点 </translation>
    </message>
    <message>
        <source>sec   at End</source>
        <translation type="obsolete">秒   終点</translation>
    </message>
    <message>
        <source>ReDef. Center:  Current Holder </source>
        <translation>原点再定義:   現在のホルダ </translation>
    </message>
    <message>
        <source>MoveTo</source>
        <translation>移動</translation>
    </message>
    <message>
        <source>  Holder No.</source>
        <translation>  ホルダ No. </translation>
    </message>
    <message>
        <source>mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source>Sample Changer</source>
        <translation>試料交換 </translation>
    </message>
    <message>
        <source>  Fine Pos.</source>
        <translation>  微調整 </translation>
    </message>
    <message>
        <source>Limi at I0 Eng.</source>
        <translation type="obsolete">I0エネルギーで制限</translation>
    </message>
    <message>
        <source>Hide T.</source>
        <translation>隠(上部)</translation>
    </message>
    <message>
        <source>Hide B.</source>
        <translation>隠(下部)</translation>
    </message>
    <message>
        <source>Datum</source>
        <translation>データム</translation>
    </message>
    <message utf8="true">
        <source>２D scan</source>
        <translation>2次元スキャン</translation>
    </message>
    <message>
        <source>Contimuous</source>
        <translation type="obsolete">連続</translation>
    </message>
    <message>
        <source>Select Detector</source>
        <translation>検出器選択</translation>
    </message>
    <message>
        <source>Setup Axis</source>
        <translation>軸設定</translation>
    </message>
    <message>
        <source>End</source>
        <translation type="obsolete">終点</translation>
    </message>
    <message>
        <source>Current</source>
        <translation>現在値</translation>
    </message>
    <message>
        <source>Pnts.</source>
        <translation type="obsolete">点数</translation>
    </message>
    <message>
        <source>Time</source>
        <translation type="obsolete">時間</translation>
    </message>
    <message>
        <source>Rel/Abs</source>
        <translation>相対/絶対</translation>
    </message>
    <message>
        <source>Ring Stat.</source>
        <translation>光源状況</translation>
    </message>
    <message>
        <source>about:blank</source>
        <translation></translation>
    </message>
    <message>
        <source>StepScn.</source>
        <translation>ステップ</translation>
    </message>
    <message>
        <source>Cnt.Scn.</source>
        <translation type="obsolete">連続</translation>
    </message>
    <message>
        <source>Periods</source>
        <translation>刻数</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;For the 1st axis, the measurement will be conducted while the axis is moving from a point to the next.&lt;/p&gt;&lt;p&gt;So that, the number of the measurements will be the same as the &apos;periods&apos;.&lt;/p&gt;&lt;p&gt;However, for the 2nd and 3rd axes, the measurement will conducted at each point.&lt;/p&gt;&lt;p&gt;So that, the number of the measurements will be &apos;periods + 1&apos;.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;第1軸については、１つの点から次の点に移動している間に測定が行われます。このため、測定値は2つの点の間の値となり、測定点数は「刻数」と一致します。
一方で、第2, 3軸については、測定は各点で行われるため、測定点数は「刻数+1」となります。
第1, 2軸ともに -10〜10 の範囲をスクエアに測定したい場合、
1軸  -11〜11, 刻み数 11 (幅2), 2軸 -10〜10, 刻数 10(幅2) の様に指定します。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>-10</source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;For the 2nd and 3rd axes, the measurement will conducted at each point.&lt;/p&gt;&lt;p&gt;So that, the number of the measurements will be &apos;periods + 1&apos;.&lt;/p&gt;&lt;p&gt;However, for the 1st axis, the measurement will be conducted while the axis is moving from a point to the next.&lt;/p&gt;&lt;p&gt;So that, the number of the measurements will be the same as the &apos;periods&apos;.&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;第1軸については、１つの点から次の点に移動している間に測定が行われます。このため、測定値は2つの点の間の値となり、測定点数は「刻数」と一致します。
一方で、第2, 3軸については、測定は各点で行われるため、測定点数は「刻数+1」となります。
第1, 2軸ともに -10〜10 の範囲をスクエアに測定したい場合、
1軸  -11〜11, 刻み数 11 (幅2), 2軸 -10〜10, 刻数 10(幅2) の様に指定します。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>  [sec]</source>
        <translation>  [秒]</translation>
    </message>
    <message>
        <source>Can&apos;t start. Othre Process is going on.</source>
        <translation>他の作業が行われているため開始できません。</translation>
    </message>
    <message>
        <source>Can&apos;t start 2D Scan. Othre Process is going on.</source>
        <translation>他の作業が行われているため2次元スキャンを開始できません。</translation>
    </message>
    <message>
        <source>2D Scan cannot Start : (%1) is disabled</source>
        <translation>[%1]が使えないため2次元スキャンを開始できません。</translation>
    </message>
    <message>
        <source>Meas Time is 0 or less.</source>
        <translation>測定時間が 0またはそれ以下になっています。</translation>
    </message>
    <message>
        <source>2D Scan Range Error.</source>
        <translation>2次元スキャンの範囲指定が誤っています。</translation>
    </message>
    <message>
        <source>2D Scan Points Error.</source>
        <translation>2次元スキャンの刻数が誤っています。</translation>
    </message>
    <message>
        <source>The same motor was selected for different axis.</source>
        <translation>スキャンのための異なる軸として同じモーターが選択されています。</translation>
    </message>
    <message>
        <source>Continuous scan is not available now.</source>
        <translation>連続測定は現在サポートされていません。</translation>
    </message>
    <message>
        <source>2D Scan Start (%1 %2 %3 (%4))</source>
        <translation>2次元スキャンを開始します。(%1 %2 %3 (%4))</translation>
    </message>
    <message>
        <source>2D Scan Finished.</source>
        <translation>2次元スキャン終了。</translation>
    </message>
    <message>
        <source>QXafs data points [%1].</source>
        <translation>QXafs データ点数 [%1].</translation>
    </message>
    <message>
        <source>Can&apos;t open QXafs data file [%1] to write data body.</source>
        <translation>QXafs のデータを書き込むためにファイル[%1]が開けません。</translation>
    </message>
    <message>
        <source>Moving</source>
        <translation>移動中</translation>
    </message>
    <message>
        <source>Monochro is used by the process %1!</source>
        <translation>分光器が[%1]に使われています。</translation>
    </message>
    <message>
        <source>The Sensor [%1] is used by the process %2!</source>
        <translation>検出器[%1]は[%2]に使われています。</translation>
    </message>
    <message>
        <source>The Motor [%1] is used by the process %2!</source>
        <translation>駆動軸[%1]は[%2]に使われています。</translation>
    </message>
    <message>
        <source>The Monochromator is used by the process %1!</source>
        <translation>分光器は[%1]に使われています。</translation>
    </message>
    <message>
        <source>The Motor [%1] is used by the process %1!</source>
        <translation type="obsolete">駆動軸[%1]は[%2]に使われています。</translation>
    </message>
    <message>
        <source>End Time</source>
        <translation>終了時間</translation>
    </message>
    <message>
        <source>Measure on Backward</source>
        <translation>戻りでも測定</translation>
    </message>
    <message>
        <source>Meas. Background Time.[s] </source>
        <translation>バックグラウンド測定時間[秒]</translation>
    </message>
    <message>
        <source>Move Monochromator</source>
        <translation>分光器回転</translation>
    </message>
    <message>
        <source>DXM center was changed from %1 to %2.</source>
        <translation>分光器の原点が%1から%2に変更されました。</translation>
    </message>
    <message>
        <source>mu2</source>
        <translation>μ2</translation>
    </message>
    <message>
        <source>26</source>
        <translation>26</translation>
    </message>
    <message>
        <source>25</source>
        <translation>25</translation>
    </message>
    <message>
        <source>27</source>
        <translation>27</translation>
    </message>
    <message>
        <source>mu3</source>
        <translation>μ3</translation>
    </message>
    <message>
        <source>At least 1 detector should be selected, except I0.</source>
        <translation>I0 以外に少なくともひとつ検出器を選択してください。</translation>
    </message>
    <message>
        <source>I1 should be selected for the measurement.</source>
        <translation>この測定のためには I1 が選択されていなければなりません。</translation>
    </message>
    <message>
        <source>Aux1 should be selected for the measurement.</source>
        <translation>この測定のためには「その他１」が選択されていなければなりません。</translation>
    </message>
    <message>
        <source>(pps) </source>
        <translation>(pps)</translation>
    </message>
    <message>
        <source>Speed</source>
        <translation>速度</translation>
    </message>
    <message>
        <source>(deg/s)</source>
        <translation>(deg/s)</translation>
    </message>
    <message>
        <source>D. Time</source>
        <translation>計測時間</translation>
    </message>
    <message>
        <source>Use Changer</source>
        <translation>チェンジャ使用</translation>
    </message>
    <message>
        <source>Changing to Q-XAFS mode.</source>
        <translation>測定モードをQ-XAFSモードにします。</translation>
    </message>
    <message>
        <source>Meas. before each Scan</source>
        <translation>毎測定前にバックグラウンド計測</translation>
    </message>
    <message>
        <source>Intervals:   Cycle</source>
        <translation>測定間隔:   １周期</translation>
    </message>
    <message>
        <source>sec   Backward</source>
        <translation>秒   復測定前</translation>
    </message>
    <message>
        <source>Identical sensor [%1:%2,%3:%4]is used as different inputs, like I0 and I1.</source>
        <translation>異なる入力に(例えばI0, I1に)同一の検出器[%1:%2, %3:%4]が使われています。</translation>
    </message>
    <message>
        <source>A1</source>
        <translation>A1</translation>
    </message>
    <message>
        <source>A2</source>
        <translation>A2</translation>
    </message>
    <message>
        <source>Moving DXM to the start point</source>
        <translation>分光器を測定開始点に移動中</translation>
    </message>
    <message>
        <source>Waiting Interval at start position</source>
        <translation>インターバル待ち(開始点)</translation>
    </message>
    <message>
        <source>Forward scan</source>
        <translation>順方向スキャン</translation>
    </message>
    <message>
        <source>Return back to the start position</source>
        <translation>開始地点へ回帰中</translation>
    </message>
    <message>
        <source>Waiting Interval at end position</source>
        <translation>インターバル待ち(終了地点)</translation>
    </message>
    <message>
        <source>Backward scan</source>
        <translation>逆方向スキャン</translation>
    </message>
    <message>
        <source>Pausing at start position</source>
        <translation>ポーズ(開始地点)</translation>
    </message>
    <message>
        <source>Pausing at end position</source>
        <translation>ポーズ(終了地点)</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;
&lt;p&gt;Recommended gas for I0 chamber.&lt;/p&gt;
&lt;p&gt;To use the recommended gas, you need to exchange it manually.&lt;/p&gt;
&lt;p&gt;Gas. No.  and Name: Transmittance of 1st order X-ray (Transmittance of 3rd order X-ray: Ratio of Sesitivities 3rd/1st)
&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;
&lt;p&gt;I0 チャンバに推奨されるガス&lt;/p&gt;
&lt;p&gt;イオンチャンバのガスは自動では変更されません。手動で切り替えてください。&lt;/p&gt;
&lt;p&gt;表示内容:: ガス番号 ガス種: 1次光の透過率 (3次光の透過率: 3次光と1次光の検出感度の比 3次/1次)
&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;
&lt;p&gt;Recommended gas for I1 chamber.&lt;/p&gt;
&lt;p&gt;To use the recommended gas, you need to exchange it manually.&lt;/p&gt;
&lt;p&gt;Gas. No.  and Name: Transmittance of 1st order X-ray (Transmittance of 3rd order X-ray: Ratio of Sesitivities 3rd/1st)
&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;
&lt;p&gt;I1 チャンバに推奨されるガス&lt;/p&gt;
&lt;p&gt;イオンチャンバのガスは自動では変更されません。手動で切り替えてください。&lt;/p&gt;
&lt;p&gt;表示内容:: ガス番号 ガス種: 1次光の透過率 (3次光の透過率: 3次光と1次光の検出感度の比 3次/1次)
&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>DTh1</source>
        <translation>⊿θ1</translation>
    </message>
    <message>
        <source>pls</source>
        <translation></translation>
    </message>
    <message>
        <source>Add the pair of current Energy and DTh1 values to the DTh1-tuning table.</source>
        <translation>現在のエネルギー値と⊿θ1の値のペアを⊿θ1のチューニングテーブルに追加します。</translation>
    </message>
    <message>
        <source>Set</source>
        <translation>設定</translation>
    </message>
    <message>
        <source>eV       Calib.</source>
        <translation>eV      較正</translation>
    </message>
    <message>
        <source>Both Dir.</source>
        <translation>往復測定</translation>
    </message>
    <message>
        <source>Start Scan</source>
        <translation>スキャン開始</translation>
    </message>
    <message>
        <source>Cannot open [%1]</source>
        <translation>[%1]が開けません</translation>
    </message>
    <message>
        <source>Q. Cnt.Scn.</source>
        <translation>擬似連続スキャン</translation>
    </message>
    <message>
        <source>R. Cnt. Scn.</source>
        <translation>連続スキャン</translation>
    </message>
    <message>
        <source>When a file name is selected, measured 2D/3D map is automatically saved to the file on a measurement.     Using the &apos;Save&apos; button, the 2D/3D map can be saved after a measurement at any time.</source>
        <translation>ファイル名が選択されているとスキャンを行った時得られたマップデータは自動的に保存されます。「保存」ボタンを押すことでスキャン後の任意の時点で得られたマップデータを保存することができます。</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Save current map which are shown.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>表示されているマップデータを保存します</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;On &apos;Step&apos; and &apos;Quasi Continuous&apos; scan, the dwell time is a time to measre each one point.&lt;/p&gt;&lt;p&gt;On &apos;Real Continuous&apos; scan, the dwell time is a time to scan 1 line along 1st axis.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>「ステップ」と「擬似連続」スキャンでは「測定時間(Dwell Time)」は、各1点を測定する時間です。「連続」スキャンでは、第1軸を始点から終点までスキャンする時間になります。</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;When the map is obtaind by using SSD, the map can be re-calculated with new ROI after the measurement.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="obsolete">SSD を使って測定して得られたマップの場合、測定後に ROI を変更して新たなマップを得ることができます。</translation>
    </message>
    <message>
        <source>Recalc w. New ROI</source>
        <translation type="obsolete">ROIの変更を許す</translation>
    </message>
    <message>
        <source>2D Scan Start (%1 %2 %3)</source>
        <translation>2D スキャン開始 (%1 %2 %3)</translation>
    </message>
    <message>
        <source>Can not find a unit ID %1 for the SPEEDS line</source>
        <translation>Can not find a unit ID %1 for the SPEEDS line</translation>
    </message>
    <message>
        <source>The scan speed %1 was limited to %2</source>
        <translation>設定しようとしたスキャンスピード[%1](pps)は[%2](pps)に制限されました</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>印刷</translation>
    </message>
    <message>
        <source>Both the name and the data is old.</source>
        <translation>ファイル名とデータの両方が古いものです。</translation>
    </message>
    <message>
        <source>The block parameter is out of range.</source>
        <translation>測定ブロックのパラメータが正しくありません。</translation>
    </message>
    <message>
        <source>Save MCAs</source>
        <translation>各MCA保存</translation>
    </message>
    <message>
        <source>For SSD</source>
        <translation>SSD使用時</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Measured data in Q-XAFS mode will be reformed into a step-scan like data stream with block parameters for step-scan and saved with an altanative file name.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>Q-XAFS の測定結果を、ステップスキャンの画面で指定したブロックパラメータで測定したステップスキャンの結果であるかのように加工したものも保存します。(ファイル名は、通常のQ-XAFS の測定結果のファイル名に &quot;-S&quot;が付いたものになります)</translation>
    </message>
    <message>
        <source>Save data also as step scan</source>
        <translation>ステップスキャン型データファイル生成</translation>
    </message>
    <message>
        <source>1step at </source>
        <translation> </translation>
    </message>
    <message>
        <source>eV is</source>
        <translation>eV での1ステップは</translation>
    </message>
    <message>
        <source>100</source>
        <translation>100</translation>
    </message>
    <message>
        <source>-100</source>
        <translation></translation>
    </message>
    <message>
        <source>Can&apos;t open QXafs data file as Step Scan [%1] to write data body.</source>
        <translation>ファイル[%1]が開けません(ステップスキャン型ファイル)。</translation>
    </message>
    <message>
        <source>eV.</source>
        <translation>eV です</translation>
    </message>
    <message>
        <source>CEY</source>
        <translation>転換電子収量</translation>
    </message>
    <message>
        <source>CAP</source>
        <translation>キャピラリ</translation>
    </message>
    <message>
        <source>Theta!</source>
        <translation>θ</translation>
    </message>
    <message>
        <source>DTh</source>
        <translation type="obsolete">Δθ</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>試料</translation>
    </message>
    <message>
        <source>Stage</source>
        <translation>ステージ</translation>
    </message>
    <message>
        <source>Phi</source>
        <translation>Φ</translation>
    </message>
    <message>
        <source>Omg</source>
        <translation>ω</translation>
    </message>
    <message>
        <source>open</source>
        <translation>開/閉</translation>
    </message>
    <message>
        <source>move</source>
        <translation>移動</translation>
    </message>
    <message>
        <source>Dth</source>
        <translation>Δθ</translation>
    </message>
    <message>
        <source>Counter</source>
        <translation>カウンタ</translation>
    </message>
    <message>
        <source>axis</source>
        <translation>軸</translation>
    </message>
    <message>
        <source>SSD Calibration</source>
        <translation>SSD較正データ取得</translation>
    </message>
    <message>
        <source>Dead Time Calib.</source>
        <translation>数え落とし補正</translation>
    </message>
    <message>
        <source>Auto Carib.</source>
        <translation>自動計測</translation>
    </message>
    <message>
        <source>Energy Calib.</source>
        <translation>エネルギー較正</translation>
    </message>
    <message>
        <source>keV</source>
        <translation>keV</translation>
    </message>
    <message>
        <source>GoThere</source>
        <translation>移動</translation>
    </message>
    <message>
        <source>Center correct.  x: </source>
        <translation>センター補正 x: </translation>
    </message>
    <message>
        <source>y: </source>
        <translation>y: </translation>
    </message>
    <message>
        <source>Series</source>
        <translation>シリーズ</translation>
    </message>
    <message>
        <source>Shutter</source>
        <translation>シャッター</translation>
    </message>
    <message>
        <source>pin hole</source>
        <translation>ピンホール</translation>
    </message>
    <message>
        <source>um</source>
        <translation>μm</translation>
    </message>
    <message>
        <source>Limit at I0 Eng.</source>
        <translation>I0のエネルギーで制限</translation>
    </message>
    <message>
        <source>Fit</source>
        <translation type="obsolete">フィッティング</translation>
    </message>
    <message>
        <source>mm  </source>
        <translation>mm</translation>
    </message>
    <message>
        <source>Some Sequence is running</source>
        <translation>別の自動シーケンス実行中です</translation>
    </message>
    <message>
        <source>Auto Calib.</source>
        <translation>自動較正</translation>
    </message>
    <message>
        <source>Auto Range</source>
        <translation>オートレンジ</translation>
    </message>
    <message>
        <source>Set Range</source>
        <translation>レンジ設定</translation>
    </message>
    <message>
        <source>Fitting</source>
        <translation>フィッティング</translation>
    </message>
    <message>
        <source>Can not save MCA spectra for 2D scan.</source>
        <translation>2DスキャンしたMCAスペクトルをセーブできませんでした。</translation>
    </message>
    <message>
        <source>No file name was selected</source>
        <translation>ファイル名が選択されていません。</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Record MCA spectra obtained at each step of the XAFS measurement is saved in separated files.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="obsolete">SSD を使ったXAFS測定の各ステップで得られたMCAスペクトルをセーブします。</translation>
    </message>
    <message>
        <source>Record MCAs</source>
        <translation type="obsolete">MCA記録</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;For the spectra with several scans, when &apos;in sep. file...&apos; is selected, each spectrum is saved in each file.&lt;/p&gt;&lt;p&gt;When &apos;in one file...&apos; is selected, one spectrum made by summing up all the spectra is saved in one file.&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>「個別ファイル」が選択されると、複数回のスキャンで
得られたスペクトルを個々に別のファイルにセーブします。
「総計ファイル」が選択されると、複数回のスキャンで
得られたデータを足しあげた一つのスペクトルがセーブされます。</translation>
    </message>
    <message>
        <source>in sep. file for each scan</source>
        <translation type="obsolete">個別ファイル</translation>
    </message>
    <message>
        <source>in one file as summed up</source>
        <translation type="obsolete">総計ファイル</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Only for spectra measured with 19 ch SSD&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>19ch SSD を使って測定した場合だけ有効です。</translation>
    </message>
    <message>
        <source>After Save</source>
        <translation>事後セーブ</translation>
    </message>
    <message>
        <source>Data Root</source>
        <translation>データルート</translation>
    </message>
    <message>
        <source>Pop Up</source>
        <translation>独立</translation>
    </message>
    <message>
        <source>Auto Set All</source>
        <translation>自動Set All</translation>
    </message>
    <message>
        <source>Show each scan</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show sum up of scans</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Start Pnt.</source>
        <translation>始点</translation>
    </message>
    <message>
        <source>End Pnt.</source>
        <translation>終点</translation>
    </message>
    <message>
        <source>can&apos;t open the file [%1] to read.</source>
        <translation>ファイル[%1]を読み込めません。</translation>
    </message>
    <message>
        <source>In the file [%1], an Offset line was not be found.</source>
        <translation>ファイル[%1]に &apos;Offset&apos; 行がありません。</translation>
    </message>
    <message>
        <source>can&apos;t open the file [%1] to write.</source>
        <translation>ファイル[%1]を書き込めません。</translation>
    </message>
</context>
<context>
    <name>PeriodicT</name>
    <message>
        <source>Periodic Table</source>
        <translation>周期表</translation>
    </message>
    <message>
        <source>H</source>
        <translation>H</translation>
    </message>
    <message>
        <source>He</source>
        <translation>He</translation>
    </message>
    <message>
        <source>Li</source>
        <translation>Li</translation>
    </message>
    <message>
        <source>Be</source>
        <translation>Be</translation>
    </message>
    <message>
        <source>B</source>
        <translation>B</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>N</source>
        <translation>N</translation>
    </message>
    <message>
        <source>O</source>
        <translation>O</translation>
    </message>
    <message>
        <source>F</source>
        <translation>F</translation>
    </message>
    <message>
        <source>Ne</source>
        <translation>Ne</translation>
    </message>
    <message>
        <source>Na</source>
        <translation>Na</translation>
    </message>
    <message>
        <source>Mg</source>
        <translation>Mg</translation>
    </message>
    <message>
        <source>Al</source>
        <translation>Al</translation>
    </message>
    <message>
        <source>Si</source>
        <translation>Si</translation>
    </message>
    <message>
        <source>P</source>
        <translation>P</translation>
    </message>
    <message>
        <source>S</source>
        <translation>S</translation>
    </message>
    <message>
        <source>Cl</source>
        <translation>Cl</translation>
    </message>
    <message>
        <source>Ar</source>
        <translation>Ar</translation>
    </message>
    <message>
        <source>K</source>
        <translation>K</translation>
    </message>
    <message>
        <source>Ca</source>
        <translation>Ca</translation>
    </message>
    <message>
        <source>Sc</source>
        <translation>Sc</translation>
    </message>
    <message>
        <source>Ti</source>
        <translation>Ti</translation>
    </message>
    <message>
        <source>V</source>
        <translation>V</translation>
    </message>
    <message>
        <source>Cr</source>
        <translation>Cr</translation>
    </message>
    <message>
        <source>Mn</source>
        <translation>Mn</translation>
    </message>
    <message>
        <source>Fe</source>
        <translation>Fe</translation>
    </message>
    <message>
        <source>Co</source>
        <translation>Co</translation>
    </message>
    <message>
        <source>Ni</source>
        <translation>Ni</translation>
    </message>
    <message>
        <source>Cu</source>
        <translation>Cu</translation>
    </message>
    <message>
        <source>Zn</source>
        <translation>Zn</translation>
    </message>
    <message>
        <source>Ga</source>
        <translation>Ga</translation>
    </message>
    <message>
        <source>Ge</source>
        <translation>Ge</translation>
    </message>
    <message>
        <source>As</source>
        <translation>As</translation>
    </message>
    <message>
        <source>Se</source>
        <translation>Se</translation>
    </message>
    <message>
        <source>Br</source>
        <translation>Br</translation>
    </message>
    <message>
        <source>Kr</source>
        <translation>Kr</translation>
    </message>
    <message>
        <source>Rb</source>
        <translation>Rb</translation>
    </message>
    <message>
        <source>Sr</source>
        <translation>Sr</translation>
    </message>
    <message>
        <source>Y</source>
        <translation>Y</translation>
    </message>
    <message>
        <source>Zr</source>
        <translation>Zr</translation>
    </message>
    <message>
        <source>Nb</source>
        <translation>Nb</translation>
    </message>
    <message>
        <source>Mo</source>
        <translation>Mo</translation>
    </message>
    <message>
        <source>Tc</source>
        <translation>Tc</translation>
    </message>
    <message>
        <source>Ru</source>
        <translation>Ru</translation>
    </message>
    <message>
        <source>Rh</source>
        <translation>Rh</translation>
    </message>
    <message>
        <source>Pd</source>
        <translation>Pb</translation>
    </message>
    <message>
        <source>Ag</source>
        <translation>Ag</translation>
    </message>
    <message>
        <source>Cd</source>
        <translation>Cd</translation>
    </message>
    <message>
        <source>In</source>
        <translation>In</translation>
    </message>
    <message>
        <source>Sn</source>
        <translation>Sn</translation>
    </message>
    <message>
        <source>Sb</source>
        <translation>Sb</translation>
    </message>
    <message>
        <source>Te</source>
        <translation>Te</translation>
    </message>
    <message>
        <source>I</source>
        <translation>I</translation>
    </message>
    <message>
        <source>Xe</source>
        <translation>Xe</translation>
    </message>
    <message>
        <source>Cs</source>
        <translation>Cs</translation>
    </message>
    <message>
        <source>Ba</source>
        <translation>Ba</translation>
    </message>
    <message>
        <source>*1</source>
        <translation>*1</translation>
    </message>
    <message>
        <source>Hf</source>
        <translation>Hf</translation>
    </message>
    <message>
        <source>Ta</source>
        <translation>Ta</translation>
    </message>
    <message>
        <source>W</source>
        <translation>W</translation>
    </message>
    <message>
        <source>Re</source>
        <translation>Re</translation>
    </message>
    <message>
        <source>Os</source>
        <translation>Os</translation>
    </message>
    <message>
        <source>Ir</source>
        <translation>Ir</translation>
    </message>
    <message>
        <source>Pt</source>
        <translation>Pt</translation>
    </message>
    <message>
        <source>Au</source>
        <translation>Au</translation>
    </message>
    <message>
        <source>Hg</source>
        <translation>Hg</translation>
    </message>
    <message>
        <source>Tl</source>
        <translation>Tl</translation>
    </message>
    <message>
        <source>Pb</source>
        <translation>Pb</translation>
    </message>
    <message>
        <source>Bi</source>
        <translation>Bi</translation>
    </message>
    <message>
        <source>Po</source>
        <translation>Po</translation>
    </message>
    <message>
        <source>At</source>
        <translation>At</translation>
    </message>
    <message>
        <source>Rn</source>
        <translation>Rn</translation>
    </message>
    <message>
        <source>Fr</source>
        <translation>Fr</translation>
    </message>
    <message>
        <source>Ra</source>
        <translation>Ra</translation>
    </message>
    <message>
        <source>*2</source>
        <translation>*2</translation>
    </message>
    <message>
        <source>Rf</source>
        <translation>Rf</translation>
    </message>
    <message>
        <source>Db</source>
        <translation>Db</translation>
    </message>
    <message>
        <source>Sg</source>
        <translation>Sg</translation>
    </message>
    <message>
        <source>Bh</source>
        <translation>Bh</translation>
    </message>
    <message>
        <source>Hs</source>
        <translation>Hs</translation>
    </message>
    <message>
        <source>Mt</source>
        <translation>Mt</translation>
    </message>
    <message>
        <source>Ds</source>
        <translation>Ds</translation>
    </message>
    <message>
        <source>Rg</source>
        <translation>Rg</translation>
    </message>
    <message>
        <source>Cn</source>
        <translation>Cn</translation>
    </message>
    <message>
        <source>La</source>
        <translation>La</translation>
    </message>
    <message>
        <source>Ce</source>
        <translation>Ce</translation>
    </message>
    <message>
        <source>Pr</source>
        <translation>Pr</translation>
    </message>
    <message>
        <source>Nd</source>
        <translation>Nd</translation>
    </message>
    <message>
        <source>Pm</source>
        <translation>Pm</translation>
    </message>
    <message>
        <source>Sm</source>
        <translation>Sm</translation>
    </message>
    <message>
        <source>Eu</source>
        <translation>Eu</translation>
    </message>
    <message>
        <source>Gd</source>
        <translation>Gd</translation>
    </message>
    <message>
        <source>Tb</source>
        <translation>Tb</translation>
    </message>
    <message>
        <source>Dy</source>
        <translation>Dy</translation>
    </message>
    <message>
        <source>Ho</source>
        <translation>Ho</translation>
    </message>
    <message>
        <source>Er</source>
        <translation>Er</translation>
    </message>
    <message>
        <source>Tm</source>
        <translation>Tm</translation>
    </message>
    <message>
        <source>Yb</source>
        <translation>Yb</translation>
    </message>
    <message>
        <source>Lu</source>
        <translation>Lu</translation>
    </message>
    <message>
        <source>Ac</source>
        <translation>Ac</translation>
    </message>
    <message>
        <source>Th</source>
        <translation>Th</translation>
    </message>
    <message>
        <source>Pa</source>
        <translation>Pa</translation>
    </message>
    <message>
        <source>U</source>
        <translation>U</translation>
    </message>
    <message>
        <source>Np</source>
        <translation>Np</translation>
    </message>
    <message>
        <source>Pu</source>
        <translation>Pu</translation>
    </message>
    <message>
        <source>Am</source>
        <translation>Am</translation>
    </message>
    <message>
        <source>Cm</source>
        <translation>Cm</translation>
    </message>
    <message>
        <source>Bk</source>
        <translation>Bk</translation>
    </message>
    <message>
        <source>Cf</source>
        <translation>Cf</translation>
    </message>
    <message>
        <source>Es</source>
        <translation>Es</translation>
    </message>
    <message>
        <source>Fm</source>
        <translation>Fm</translation>
    </message>
    <message>
        <source>Md</source>
        <translation>Md</translation>
    </message>
    <message>
        <source>No</source>
        <translation>No</translation>
    </message>
    <message>
        <source>Lr</source>
        <translation>Lr</translation>
    </message>
    <message>
        <source>All</source>
        <translation>全選択</translation>
    </message>
    <message>
        <source>None</source>
        <translation>全解除</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>閉じる</translation>
    </message>
</context>
<context>
    <name>PeriodicTable</name>
    <message>
        <source>Atom No. : %1</source>
        <translation>原子番号: %1</translation>
    </message>
    <message>
        <source>
 %1-edge : %2 keV</source>
        <translation>
 %1-edge : %2 keV</translation>
    </message>
    <message>
        <source>
   %1 : %2 keV</source>
        <translation>
   %1 : %2 keV</translation>
    </message>
</context>
<context>
    <name>RelAbs</name>
    <message>
        <source>Form</source>
        <translation></translation>
    </message>
    <message>
        <source>Rel</source>
        <translation>相対</translation>
    </message>
    <message>
        <source>Abs</source>
        <translation>絶対</translation>
    </message>
</context>
<context>
    <name>S2DView</name>
    <message>
        <source>Frame</source>
        <translation></translation>
    </message>
    <message>
        <source>Int. MCA</source>
        <translation>積分MCA</translation>
    </message>
    <message>
        <source>Inv. X</source>
        <translation>X軸反転</translation>
    </message>
    <message>
        <source>Inv. Y</source>
        <translation>Y軸反転</translation>
    </message>
    <message>
        <source>Measured : (%1, %2)</source>
        <translation>現在の測定点 : (%1, %2)</translation>
    </message>
    <message>
        <source>Pinted : (%1, %2)</source>
        <translation>指定点 : (%1, %2)</translation>
    </message>
    <message>
        <source>Intensity : %1</source>
        <translation>測定強度 : %1</translation>
    </message>
</context>
<context>
    <name>ScrollWGrid</name>
    <message>
        <source>ScrollArea</source>
        <translation>ScrollArea</translation>
    </message>
</context>
<context>
    <name>SelMC</name>
    <message>
        <source>Set Monochro Crystal</source>
        <translation type="obsolete">分光結晶情報設定</translation>
    </message>
    <message>
        <source>Monochro Crystal</source>
        <translation type="obsolete">分光結晶</translation>
    </message>
    <message>
        <source>3.1355316 A</source>
        <translation type="obsolete">3.1355316 A</translation>
    </message>
    <message>
        <source>Change</source>
        <translation type="obsolete">変更</translation>
    </message>
    <message>
        <source>%1 A</source>
        <translation type="obsolete">%1 A</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">中止</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>&lt;h1&gt;&lt;center&gt;Do you really change it ?&lt;/center&gt;&lt;/h1&gt;</source>
        <translation type="obsolete">&lt;h1&gt;&lt;center&gt;本当に変更しますか?&lt;/center&gt;&lt;/h1&gt;</translation>
    </message>
    <message>
        <source>Really Change?</source>
        <translation type="obsolete">本当に変更しますか?</translation>
    </message>
    <message>
        <source>Mono: Cnaged to %1 (%2 A)</source>
        <translation type="obsolete">分光結晶: 変更 %1 (%2 A)</translation>
    </message>
</context>
<context>
    <name>SelMC2</name>
    <message>
        <source>Frame</source>
        <translation></translation>
    </message>
    <message>
        <source>Change</source>
        <translation>変更</translation>
    </message>
    <message>
        <source>Monochro Crystal</source>
        <translation>分光結晶</translation>
    </message>
    <message>
        <source>3.1355316 A</source>
        <translation>3.1355316 A</translation>
    </message>
    <message>
        <source>%1 A</source>
        <translation>%1 A</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancel</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>&lt;h1&gt;&lt;center&gt;Do you really change it ?&lt;/center&gt;&lt;/h1&gt;</source>
        <translation>&lt;h1&gt;&lt;center&gt;本当に変更しますか?&lt;/center&gt;&lt;/h1&gt;</translation>
    </message>
    <message>
        <source>Really Change?</source>
        <translation>本当に変更しますか?</translation>
    </message>
    <message>
        <source>Mono: Cnaged to %1 (%2 A)</source>
        <translation>分光結晶: 変更 %1 (%2 A)</translation>
    </message>
</context>
<context>
    <name>Stars</name>
    <message>
        <source>Cannot open [%1.key]</source>
        <translation>Cannot open [%1.key]</translation>
    </message>
    <message>
        <source>Cannot open both [%1.key] and [:%2.key]</source>
        <translation>Cannot open both [%1.key] and [:%2.key]</translation>
    </message>
    <message>
        <source>Connecting Stars Server [%1] [%2] as [%3]</source>
        <translation>Starsサーバ [%1][%2] に [%3] として接続できませんでした。</translation>
    </message>
    <message>
        <source>%1 %2
</source>
        <translation></translation>
    </message>
    <message>
        <source>Success to connect.</source>
        <translation>Stars サーバとの接続成功</translation>
    </message>
    <message>
        <source>Receive an answer from Stars [%1]</source>
        <translation>Stars サーバからのメッセージ [%1]</translation>
    </message>
    <message>
        <source>Receive an event from Stars [%1]</source>
        <translation>Stars サーバからのイベント [%1]</translation>
    </message>
    <message>
        <source>Receive an unricognized message from Stars [%1]</source>
        <translation>Stars サーバからの解釈できないメッセージ [%1]</translation>
    </message>
    <message>
        <source>Others:  [%1] -&gt; [%2] : [%3] [%4]
</source>
        <translation>Others:  [%1] -&gt; [%2] : [%3] [%4</translation>
    </message>
    <message>
        <source>Sending a message [%1] to Stars</source>
        <translation>Stars サーバにメッセージ送信: [%1]</translation>
    </message>
</context>
<context>
    <name>StarsSV</name>
    <message>
        <source>Set Stars Server</source>
        <translation type="obsolete">Stars サーバ情報設定</translation>
    </message>
    <message>
        <source>Stars Server Address: </source>
        <translation type="obsolete">Starsサーバアドレス: </translation>
    </message>
    <message>
        <source>Port No.:</source>
        <translation type="obsolete">ポート番号: </translation>
    </message>
    <message>
        <source>ReConnect</source>
        <translation type="obsolete">再接続</translation>
    </message>
</context>
<context>
    <name>StarsSV2</name>
    <message>
        <source>Frame</source>
        <translation>Frame</translation>
    </message>
    <message>
        <source>Stars Server Address </source>
        <translation>Starsサーバアドレス</translation>
    </message>
    <message>
        <source>Port No.</source>
        <translation>ポート番号</translation>
    </message>
    <message>
        <source>ReConnect</source>
        <translation>再接続</translation>
    </message>
</context>
<context>
    <name>Status</name>
    <message>
        <source>Stars server :</source>
        <translation type="obsolete">Starsサーバ</translation>
    </message>
    <message>
        <source>SS Address</source>
        <translation type="obsolete">サーバアドレス</translation>
    </message>
    <message>
        <source>SS Port</source>
        <translation type="obsolete">サーバポート</translation>
    </message>
    <message>
        <source>not connected</source>
        <translation type="obsolete">未接続</translation>
    </message>
    <message>
        <source>: Drivers status watch active</source>
        <translation>ドライバ状態監視</translation>
    </message>
    <message>
        <source>Drivers</source>
        <translation>ドライバ</translation>
    </message>
    <message>
        <source>Devices</source>
        <translation>デバイス</translation>
    </message>
    <message>
        <source>Enable</source>
        <translation>Enable</translation>
    </message>
    <message>
        <source>Clr. Enable</source>
        <translation>Clr. Enable</translation>
    </message>
    <message>
        <source>Clr. Busys</source>
        <translation>Clr. Busys</translation>
    </message>
    <message>
        <source>Clear Enable</source>
        <translation type="obsolete">Clear Enable</translation>
    </message>
    <message>
        <source>IsBusy</source>
        <translation>IsBusy</translation>
    </message>
    <message>
        <source>Busy Units</source>
        <translation>Busy Units</translation>
    </message>
    <message>
        <source>IsBusy2</source>
        <translation>IsBusy2</translation>
    </message>
    <message>
        <source>Busy2 Units</source>
        <translation>Busy2 Units</translation>
    </message>
    <message>
        <source>Clear Busys</source>
        <translation type="obsolete">Clear Busys</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>クリア</translation>
    </message>
    <message>
        <source>Encorder</source>
        <translation>エンコーダ</translation>
    </message>
    <message>
        <source>   </source>
        <translation></translation>
    </message>
    <message>
        <source>Set</source>
        <translation>設定</translation>
    </message>
    <message>
        <source>Connected</source>
        <translation>接続</translation>
    </message>
    <message>
        <source>not Connected</source>
        <translation>未接続</translation>
    </message>
</context>
<context>
    <name>TYView</name>
    <message>
        <source>By clicking the &apos;A. Scale&apos; (Auto Scale) button at left-bottom corner, the auto-scale mode is toggled on and off.</source>
        <translation>左下の「自動スケール」と書かれたボタンを押すことで、オートスケールモードをオン・オフできます。</translation>
    </message>
    <message>
        <source>%1</source>
        <translation>%1</translation>
    </message>
    <message>
        <source>A. Scale</source>
        <translation>自動スケール</translation>
    </message>
    <message>
        <source>L. Scale</source>
        <translation>Log表示</translation>
    </message>
</context>
<context>
    <name>TuneTable</name>
    <message>
        <source>Frame</source>
        <translation></translation>
    </message>
    <message>
        <source>Both of the Calibratio Energy and the Delta Th1 value can be edited.    When the Calibration Energy set to be 0, the line is deleted when the editing window is closed by pushing the OK button.</source>
        <translation>較正を行うエネルギーとその時の⊿θ1の値のいずれも編集可能です(行の追加も可能)。エネルギーの値を0にしておくと、「OK」を押してウインドウを閉じた時、その行は削除されます。</translation>
    </message>
    <message>
        <source>Calibration Energy [keV]</source>
        <translation>較正エネルギー [keV]</translation>
    </message>
    <message>
        <source>Delta Th1 value [pulse]</source>
        <translation>⊿θ1の値 [pulse]</translation>
    </message>
    <message>
        <source>Ok</source>
        <translation></translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>XView</name>
    <message>
        <source>Frame</source>
        <translation>Frame</translation>
    </message>
    <message>
        <source>%1</source>
        <translation type="obsolete">%1</translation>
    </message>
</context>
<context>
    <name>XYView</name>
    <message>
        <source>By clicking the &apos;A. Scale&apos; (Auto Scale) button at left-bottom corner, the auto-scale mode is toggled on and off.</source>
        <translation>左下の「自動スケール」 と書かれたボタンを押すことで、オートスケールモードをオン・オフできます。</translation>
    </message>
    <message>
        <source>A. Scale</source>
        <translation>自動スケール</translation>
    </message>
    <message>
        <source>S. Scale</source>
        <translation>単一スケール</translation>
    </message>
    <message>
        <source>Diff1</source>
        <translation>1階微分</translation>
    </message>
    <message>
        <source>Diff2</source>
        <translation>2階微分</translation>
    </message>
</context>
<context>
    <name>cBar</name>
    <message>
        <source>Frame</source>
        <translation>Frame</translation>
    </message>
</context>
</TS>
