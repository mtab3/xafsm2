<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="ja_JP">
<context>
    <name>AUnit</name>
    <message>
        <location filename="AUnit.cpp" line="278"/>
        <source> TP[%1] ID[%2] NM[%3] DR[%4] Ch[%5] DC[%6]  UT[%7] PU[%8] CT[%9] MaV[%10] MiV[%11] </source>
        <translation>TP[%1] ID[%2] NM[%3] DR[%4] Ch[%5] DC[%6]  UT[%7] PU[%8] CT[%9] MaV[%10] MiV[%11]</translation>
    </message>
</context>
<context>
    <name>Conditions</name>
    <message>
        <location filename="Conditions.ui" line="14"/>
        <source>Frame</source>
        <translation>Frame</translation>
    </message>
    <message>
        <location filename="Conditions.ui" line="50"/>
        <source>Encorder is always used for Recorded Th</source>
        <translation>θとして常にエンコーダの値を記録する</translation>
    </message>
    <message>
        <location filename="Conditions.ui" line="60"/>
        <source>Selected Encorder or &apos;calc from PM&apos; is used</source>
        <translation>エンコーダかパルスモータの換算値かを選択する</translation>
    </message>
    <message>
        <source>No normalization</source>
        <translation type="obsolete">計測時間の補正無し</translation>
    </message>
    <message>
        <location filename="Conditions.ui" line="91"/>
        <source>No normalization    (now &apos;No&apos; shold be selected always)</source>
        <translation>規格化なし(今は、つねにこれを選んで下さい)</translation>
    </message>
    <message>
        <location filename="Conditions.ui" line="101"/>
        <source>Normalize the SSD-measured value by RealTime</source>
        <translation>SSDの測定値を「RealTime」で規格化する</translation>
    </message>
    <message>
        <location filename="Conditions.ui" line="108"/>
        <source>Normalize the SSD-measured value by LiveTime</source>
        <translation>SSDの測定値を「LiveTime」で規格化する</translation>
    </message>
    <message>
        <location filename="Conditions.ui" line="136"/>
        <location filename="Conditions.ui" line="199"/>
        <source>Some additional information is recorded 
with usual 9809 format datas.</source>
        <translation>9809形式の通常のデータに加えて、追加の情報を行末に記録する(エンコーダ読み、パルスモータ換算両方の角度とエネルギー)。</translation>
    </message>
    <message>
        <location filename="Conditions.ui" line="140"/>
        <source>Record Addicional Informations in &quot;.dat&quot; XAFS file</source>
        <translation>XAFS 測定の &quot;.dat&quot; ファイルに付加的な情報を含める。</translation>
    </message>
    <message>
        <location filename="Conditions.ui" line="203"/>
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
        <location filename="Conditions.ui" line="168"/>
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
</context>
<context>
    <name>DarkTable</name>
    <message>
        <location filename="DarkTable.ui" line="14"/>
        <source>Frame</source>
        <translation>バックグラウンド</translation>
    </message>
    <message>
        <location filename="DarkTable.ui" line="32"/>
        <source>Close</source>
        <translation>閉じる</translation>
    </message>
    <message>
        <location filename="DarkTable.ui" line="180"/>
        <source>Sensor Name</source>
        <translation>検出器名</translation>
    </message>
    <message>
        <location filename="DarkTable.ui" line="185"/>
        <source>Dark Value</source>
        <translation>バックグラウンド値</translation>
    </message>
</context>
<context>
    <name>Data</name>
    <message>
        <location filename="Data.ui" line="14"/>
        <source>Frame</source>
        <translation>Frame</translation>
    </message>
    <message>
        <location filename="Data.ui" line="126"/>
        <source>Show</source>
        <translation>表示</translation>
    </message>
    <message>
        <location filename="Data.ui" line="139"/>
        <source>  Type : </source>
        <translation>タイプ : </translation>
    </message>
    <message>
        <location filename="Data.ui" line="301"/>
        <source>Select</source>
        <translation>選択</translation>
    </message>
    <message>
        <location filename="Data.cpp" line="39"/>
        <source>Measured</source>
        <translation>XAFS測定</translation>
    </message>
    <message>
        <location filename="Data.cpp" line="39"/>
        <source>Scaned</source>
        <translation>スキャン</translation>
    </message>
    <message>
        <location filename="Data.cpp" line="40"/>
        <source>Monitored</source>
        <translation>モニタ</translation>
    </message>
    <message>
        <location filename="Data.cpp" line="40"/>
        <source>MCA</source>
        <translation>MCA(SSD)</translation>
    </message>
    <message>
        <location filename="Data.cpp" line="102"/>
        <location filename="Data.cpp" line="151"/>
        <source>Can not open the file %1.</source>
        <translation>ファイル [%1] を開けません。</translation>
    </message>
    <message>
        <location filename="Data.cpp" line="110"/>
        <source>The file %1 is empty.</source>
        <translation>ファイル [%1] は空です。</translation>
    </message>
    <message>
        <location filename="Data.cpp" line="134"/>
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
        <location filename="MCAView.ui" line="17"/>
        <source>Frame</source>
        <translation>Frame</translation>
    </message>
    <message>
        <location filename="MCAView.cpp" line="343"/>
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
        <location filename="MCAView.cpp" line="353"/>
        <source>Cursor</source>
        <translation>カーソル</translation>
    </message>
    <message>
        <location filename="MCAView.cpp" line="359"/>
        <source> Pos. [keV] : </source>
        <translation> 位置[keV] : </translation>
    </message>
    <message>
        <location filename="MCAView.cpp" line="368"/>
        <source> Pos. [ch] : </source>
        <translation> 位置[ch] : </translation>
    </message>
    <message>
        <location filename="MCAView.cpp" line="377"/>
        <source> Val. : </source>
        <translation> 値 : </translation>
    </message>
    <message>
        <location filename="MCAView.cpp" line="386"/>
        <source>ROI </source>
        <translation>ROI </translation>
    </message>
    <message>
        <location filename="MCAView.cpp" line="392"/>
        <source> Start [keV] : </source>
        <translation> 始点[keV] : </translation>
    </message>
    <message>
        <location filename="MCAView.cpp" line="401"/>
        <source> End [keV] : </source>
        <translation> 終点[keV] : </translation>
    </message>
    <message>
        <location filename="MCAView.cpp" line="410"/>
        <source> Count : </source>
        <translation> カウント : </translation>
    </message>
    <message>
        <location filename="MCAView.cpp" line="419"/>
        <source>   CPS : </source>
        <translation>   CPS :</translation>
    </message>
    <message>
        <location filename="MCAView.cpp" line="428"/>
        <source>Real Time : </source>
        <translation>実時間: </translation>
    </message>
    <message>
        <location filename="MCAView.cpp" line="437"/>
        <source>Live Time : </source>
        <translation>ライブ時間: </translation>
    </message>
    <message>
        <location filename="MCAView.cpp" line="459"/>
        <source>Dead Time : </source>
        <translation>デッドタイム: </translation>
    </message>
</context>
<context>
    <name>MUnits</name>
    <message>
        <location filename="MultiUnits.cpp" line="98"/>
        <location filename="MultiUnits.cpp" line="114"/>
        <source>Dwell time was set [%1] for [%2], though tried to be as [%3].</source>
        <translation>検出器[%2]の計測時間を[%3]に設定しようとしましたが、実際には[%1]に設定されました。</translation>
    </message>
    <message>
        <location filename="MultiUnits.cpp" line="102"/>
        <location filename="MultiUnits.cpp" line="118"/>
        <source>Warning on dwell time</source>
        <translation>計測時間に関する警告</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="MainWindow.ui" line="20"/>
        <source>MainWindow</source>
        <translation>XAFS測定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="448"/>
        <source>View 1</source>
        <translation>View1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="454"/>
        <source>View 2</source>
        <translation>View2</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="460"/>
        <source>View 3</source>
        <translation>View3</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="466"/>
        <source>View 4</source>
        <translation>View4</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="472"/>
        <source>View5</source>
        <translation>View5</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="478"/>
        <source>View6</source>
        <translation>View6</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="484"/>
        <source>View7</source>
        <translation>View7</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="490"/>
        <source>View8</source>
        <translation>View8</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="496"/>
        <source>View9</source>
        <translation>View9</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="502"/>
        <source>View10</source>
        <translation>View10</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="242"/>
        <source>Select an Atom</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="245"/>
        <source>Select a Target Atom</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="264"/>
        <source>Periodic Table</source>
        <translation>周期表</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="277"/>
        <source>Target</source>
        <translation>対象元素</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="290"/>
        <source>Edge</source>
        <translation>対象殻</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="306"/>
        <source>Select an absorption edge</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="318"/>
        <source>  Edge</source>
        <translation>エッジ位置</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="123"/>
        <source>KeV  </source>
        <translation>keV</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="370"/>
        <source>Deg.</source>
        <translation>度</translation>
    </message>
    <message>
        <source>KeV</source>
        <translation type="obsolete">keV</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="116"/>
        <source>Deg.  </source>
        <translation>度</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="109"/>
        <location filename="MainWindow.ui" line="139"/>
        <source>10.0000</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="426"/>
        <source>.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="53"/>
        <source>Cur. Pos.: </source>
        <translation>現在位置</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="75"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Usually the &amp;quot;Encorder&amp;quot; should be selected.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;通常は &amp;quot;エンコーダ&amp;quot;を選択&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="78"/>
        <source>Encorder</source>
        <translation>エンコーダ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="88"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;The &amp;quot;Calc from Pulse&amp;quot; should not be used for usual measurement.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;パルスモータのパルスから計算した角度。通常は&amp;quot;エンコーダ&amp;quot;を選択&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="91"/>
        <source>Calc from Pulse</source>
        <translation>パルス換算</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="195"/>
        <source>mA</source>
        <translation>mA</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="202"/>
        <source>RIng Current</source>
        <translation>リング電流</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="390"/>
        <source>Hide Tabs of Measurements and Settings</source>
        <translation>測定・條件設定のタブ部分を隠します</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="393"/>
        <source>Hide</source>
        <translation>隠</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="530"/>
        <source>Measurement</source>
        <translation>XAFS測定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="945"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Normally the dwell-inputs indicate dwell-times for each measurement step,&lt;/p&gt;&lt;p&gt;but in the Q-XAFS mode, it indicates total measurement time.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>通常のXAFS測定では「計測時間」は１点の測定時間を、
Q-XAFSモードでは１スキャンに必要な時間を表します。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1292"/>
        <source>Mesure on Backward</source>
        <translation>戻りでも測定を行う</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1573"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Select an ion-chamber for I0.   The recommended gas is displayed in the following field.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>I0 の計測に使うイオンチャンバを選ぶと、推奨される混合ガスが次の欄に表示されます。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1580"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Recommended gas for I0 chamber.&lt;/p&gt;&lt;p&gt;To use the recommended gas, you need to exchange it manually.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>I0 チャンバに使用するお勧め混合ガス。
表示されたガスに自動的には切り替わらないので手動で切り替える必要があります。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1587"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Select an ion-chamber for I1.   The recommended gas is displayed in the following field.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>I1 の計測に使うイオンチャンバを選ぶと、推奨される混合ガスが次の欄に表示されます。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1594"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Recommended gas for I1 chamber.&lt;/p&gt;&lt;p&gt;To use the recommended gas, you need to exchange it manually.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>I1 チャンバに使用するお勧め混合ガス。
表示されたガスに自動的には切り替わらないので手動で切り替える必要があります。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1695"/>
        <source>Range of the device selected to measure A1.</source>
        <translation>「その他1」を測定する検出器のレンジ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1756"/>
        <source>Range of the device selected to measure I0.</source>
        <translation>I0 を測定する検出器のレンジ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1772"/>
        <source>Range of the device selected to measure I1.</source>
        <translation>I1 を測定する検出器のレンジ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1801"/>
        <source>Range of the device selected to measure A2.</source>
        <translation>「その他2」を測定する検出器のレンジ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1837"/>
        <source>Use novel 1303 data format to record the measured data instead of the usual 9809 format.</source>
        <translation>従来の 9809 フォーマットの代わりに 1303 フォーマットを用いて測定結果を記録する。</translation>
    </message>
    <message>
        <source>Use 1303 Format</source>
        <translation type="obsolete">1303フォーマット使用</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1959"/>
        <source>When 19ch SSD is used for the measurement, the MCA spectra for each channel is recorded at every measured points.</source>
        <translation>19ch SSDを使用する際、測定の各ステップでの MCA スペクトルを全て(全チャンネル、全ステップ)記録する。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1962"/>
        <source>Record MCA Spectra</source>
        <translation>MCAスペクトル記録</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="705"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;To measure near edge region precisely.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;XANES: ニアエッジ領域を詳細に測定&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="711"/>
        <source>Std. XANES</source>
        <translation>標準XANES</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="718"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Both EXAFS and XAFNES regions are measured precisely at once.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;XAFS: XANES領域とEXAFS領域の両方を詳細に測定&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="724"/>
        <source>Std. XAFS</source>
        <translation>標準XAFS</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="749"/>
        <location filename="MainWindow.ui" line="2668"/>
        <location filename="MainWindow.ui" line="3607"/>
        <location filename="MainWindow.ui" line="5080"/>
        <source>Save</source>
        <translation>保存</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="774"/>
        <source>Load</source>
        <translation>読込</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="787"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;To measure focusing on the &apos;EXAFS&apos; region. &lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;EXAFS: EXAFS領域に重点を置いた測定&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="793"/>
        <source>Std. EXAFS</source>
        <translation>標準EXAFS</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="844"/>
        <location filename="MainWindow.ui" line="4146"/>
        <location filename="MainWindow.ui" line="5479"/>
        <source>1</source>
        <translation>1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="864"/>
        <location filename="MainWindow.ui" line="4482"/>
        <location filename="MainWindow.ui" line="5528"/>
        <source>2</source>
        <translation>2</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="888"/>
        <location filename="MainWindow.ui" line="4165"/>
        <location filename="MainWindow.ui" line="5242"/>
        <source>3</source>
        <translation>3</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1059"/>
        <location filename="MainWindow.ui" line="4184"/>
        <location filename="MainWindow.ui" line="5292"/>
        <source>4</source>
        <translation>4</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1083"/>
        <location filename="MainWindow.ui" line="4203"/>
        <location filename="MainWindow.ui" line="5305"/>
        <source>5</source>
        <translation>5</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1121"/>
        <location filename="MainWindow.ui" line="4222"/>
        <location filename="MainWindow.ui" line="5318"/>
        <source>6</source>
        <translation>6</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1254"/>
        <location filename="MainWindow.ui" line="4241"/>
        <location filename="MainWindow.ui" line="5331"/>
        <source>7</source>
        <translation>7</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1160"/>
        <source>StartP</source>
        <translation>始点</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1179"/>
        <source>Step</source>
        <translation>間隔</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1192"/>
        <source>Steps</source>
        <translation>刻数</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="935"/>
        <source>Blocks: </source>
        <translation>ブロック数</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="948"/>
        <source>Dwell [s]</source>
        <translation>計測時間[秒]</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="997"/>
        <source>Set all dwell times at once</source>
        <translation>全計測時間を一斉に設定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1007"/>
        <source>All: </source>
        <translation>All: </translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2191"/>
        <source>Data File Name</source>
        <translation>データファイル</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2220"/>
        <source>...</source>
        <translation>選択</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2231"/>
        <source>Comment</source>
        <translation>コメント</translation>
    </message>
    <message>
        <source>Detecotr Select</source>
        <translation type="obsolete">検出器選択</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1526"/>
        <source>    I0</source>
        <translation>　I0</translation>
    </message>
    <message>
        <source>I</source>
        <translation type="obsolete">I</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1644"/>
        <source>Aux. 1</source>
        <translation>その他 1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1660"/>
        <source>Aux. 2</source>
        <translation>その他 2</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1516"/>
        <source>19ch SSD</source>
        <translation>19ch SSD</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1883"/>
        <source>The change of the repitation No. is available, even after starting measurement.
Though, other parameters are fixed at the value on starting.</source>
        <translation>「繰返し回数」は測定開始後も変更可能(その他の測定に関わるパラメータは測定後に変更しても反映されない)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1900"/>
        <source>Repeat</source>
        <translation>繰返し</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1917"/>
        <source>On Finish</source>
        <translation>終了時動作</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1936"/>
        <location filename="MainWindow.ui" line="4610"/>
        <location filename="Measurement.cpp" line="123"/>
        <location filename="MwMeas.cpp" line="1115"/>
        <location filename="MwSSDSetUp.cpp" line="476"/>
        <location filename="QXafs.cpp" line="191"/>
        <source>Start</source>
        <translation>開始</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1952"/>
        <location filename="MwMeas.cpp" line="1119"/>
        <location filename="MwMeas.cpp" line="1142"/>
        <location filename="MwMeas.cpp" line="1188"/>
        <source>Pause</source>
        <translation>一時停止</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1337"/>
        <source>Check Cond.</source>
        <translation>条件確認</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1344"/>
        <source>Total points which will be measured</source>
        <translation>総測定点数</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1347"/>
        <source>Total points which will be measured
</source>
        <translation>総測定点数</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1360"/>
        <source>Points:</source>
        <translation>点数:</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1367"/>
        <source>Total measurement time</source>
        <translation>予想測定時間</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1379"/>
        <source>Time:</source>
        <translation>時間:</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2038"/>
        <source>Meas. Bacground Time.[s] </source>
        <translation>バックグラウンド計測時間 [s] </translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2139"/>
        <source>Meas. Background</source>
        <translation>バックグラウンド計測</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2103"/>
        <source>Meas. before Scan</source>
        <translation>測定前にバックグラウンド計測</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2073"/>
        <source>Show BGs</source>
        <oldsource>Show Measured</oldsource>
        <translation>計測値表示</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="743"/>
        <source>Save the block definitions.</source>
        <translation>測定ブロック設定のセーブ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="768"/>
        <source>Load the saved block definitions.</source>
        <translation>測定ブロック設定の読込</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="588"/>
        <source>Start Time</source>
        <translation>開始時間</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="617"/>
        <source>Now</source>
        <translation>現在時間</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="630"/>
        <source>End TIme</source>
        <translation>終了時間</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1840"/>
        <source>Q-XAFS mode</source>
        <translation>Q-XAFS モード</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4502"/>
        <location filename="MainWindow.ui" line="5584"/>
        <source>Down</source>
        <translation>下流</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4522"/>
        <location filename="MainWindow.ui" line="5604"/>
        <source>Stream</source>
        <translation> </translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4542"/>
        <location filename="MainWindow.ui" line="5624"/>
        <source>Up</source>
        <translation>上流</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5130"/>
        <source>Show Energy</source>
        <translation>エネルギー表示</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5140"/>
        <source>Fit</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5194"/>
        <source>ROI changeable even while the XAFS measurements</source>
        <translation>XAFS測定中もROIの変更を可能にする</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6245"/>
        <source>21</source>
        <translation>21</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6264"/>
        <source>23</source>
        <translation>23</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6283"/>
        <source>02</source>
        <translation>02</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6302"/>
        <source>22</source>
        <translation>22</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6321"/>
        <source>19</source>
        <translation>19</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6454"/>
        <source>03</source>
        <translation>03</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6062"/>
        <source>09</source>
        <translation>09</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6081"/>
        <source>08</source>
        <translation>08</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6100"/>
        <source>07</source>
        <translation>07</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6119"/>
        <source>06</source>
        <translation>06</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6138"/>
        <source>05</source>
        <translation>05</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6163"/>
        <source>24</source>
        <translation>24</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6185"/>
        <source>01</source>
        <translation>01</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6204"/>
        <source>04</source>
        <translation>04</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6226"/>
        <source>20</source>
        <translation>20</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2300"/>
        <source>Set up Conditions</source>
        <translation>条件設定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2953"/>
        <location filename="MainWindow.ui" line="2982"/>
        <location filename="MainWindow.ui" line="3025"/>
        <location filename="MainWindow.ui" line="3084"/>
        <location filename="MainWindow.ui" line="3402"/>
        <location filename="MwSetup.cpp" line="550"/>
        <source>Go</source>
        <translation>移動</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2989"/>
        <source>Move To (2)</source>
        <translation>移動先 (2)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3032"/>
        <source>Move To (3)</source>
        <translation>移動先 (3)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3091"/>
        <source>Move To (4)</source>
        <translation>移動先 (4)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3114"/>
        <source> Speed </source>
        <translation>スピード</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3176"/>
        <source>H</source>
        <translation>H</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3210"/>
        <source>M</source>
        <translation>M</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3247"/>
        <source>L</source>
        <translation>L</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3272"/>
        <source>Move Monochro</source>
        <translation>分光器回転</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3294"/>
        <source>Move To (1)</source>
        <translation>移動先 (1)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2579"/>
        <source>0.1</source>
        <translation>0.1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2557"/>
        <source>Value</source>
        <translation>計測値</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2550"/>
        <source>Dwell T.</source>
        <translation>計測時間</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2649"/>
        <location filename="MainWindow.ui" line="3648"/>
        <location filename="MainWindow.ui" line="5066"/>
        <source>Select</source>
        <translation>選択</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2601"/>
        <location filename="MwSetup.cpp" line="794"/>
        <source>Mon. Start</source>
        <translation>モニタ開始</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2515"/>
        <location filename="MainWindow.ui" line="2692"/>
        <location filename="MainWindow.ui" line="2712"/>
        <source>0.000</source>
        <translation>0.000</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2537"/>
        <source>Monitor Sensors</source>
        <translation>検出器モニタ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3728"/>
        <source> Dwell [s]:</source>
        <translation>計測時間[s]:</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3784"/>
        <location filename="Scan.cpp" line="76"/>
        <source>Scan</source>
        <translation>スキャン開始</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3814"/>
        <source>StartP:</source>
        <translation>始点:</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3568"/>
        <location filename="MainWindow.ui" line="3721"/>
        <source>Abs</source>
        <translation>絶対</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3588"/>
        <source>EndP:</source>
        <translation>終点:</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3824"/>
        <source>Step:</source>
        <translation>間隔:</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3677"/>
        <location filename="MainWindow.ui" line="3699"/>
        <source>Rel</source>
        <translation>相対</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3409"/>
        <location filename="MainWindow.ui" line="5026"/>
        <source>Save:</source>
        <translation>保存ファイル:</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3419"/>
        <source>puls</source>
        <translation>puls</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3451"/>
        <source>Move To</source>
        <translation>移動先</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3484"/>
        <source>unit</source>
        <translation>単位</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3513"/>
        <source>Position</source>
        <translation>現在位置</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3530"/>
        <source>Monitor:</source>
        <translation>検出器:</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3908"/>
        <location filename="MainWindow.ui" line="3939"/>
        <location filename="MainWindow.ui" line="4106"/>
        <location filename="MainWindow.ui" line="5267"/>
        <source>0</source>
        <translation>0</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3859"/>
        <source>Scan/Move a Motor</source>
        <translation>移動/スキャン</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="344"/>
        <source>KeV   </source>
        <translation>keV</translation>
    </message>
    <message>
        <source>Measure before Scan</source>
        <translation type="obsolete">スキャン前に計測</translation>
    </message>
    <message>
        <location filename="Dark.cpp" line="161"/>
        <location filename="MwMeas.cpp" line="1103"/>
        <source>Measure Background</source>
        <oldsource>Back Ground</oldsource>
        <translation>バックグラウンド計測</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2119"/>
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
        <location filename="MainWindow.ui" line="2132"/>
        <source>When it is displayed as &quot;Shutter Close!&quot;, check the shutter is closed and push the button.
When it is displayed as &quot;Shutter Open!, check the shutter is opend and push the button.</source>
        <translation>「シャッター CLOSE 確認」と表示されたときは、シャッターが閉まっていることを確認してボタンを押して下さい。
「シャッター OPEN 確認」と表示されたときは、シャッターが開いていることを確認してボタンを押して下さい。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2010"/>
        <source>Back ground</source>
        <translation>バックグラウンド</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3960"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;When the &apos;Normalize&apos; is checked, the value of the detector selected as &apos;Monitor&apos; is normalized by the value of the detecter selected in the following selection-box.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;「規格化」がチェックされていると、「計測器:」で選択した計測器の値を「規格化」で選択した計測器の値で規格化します。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3963"/>
        <source>Normalize</source>
        <translation>規格化</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3976"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;The value of the selected detecter is always (with or without check the &apos;Normalize&apos;) recorded with the value of the detecter selected as &apos;Monitor&apos; for the scan.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>「規格化」で選択した測定器の値は、「規格化」をチェックしていてもしていなくても、主たる計測対象の「計測器：」で選択した機器の測定値と共に常に測定ファイルに記録されます。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2312"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Select measurement renge of the sensor.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;測定器の測定レンジを選択して下さい。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2348"/>
        <source>Select Range</source>
        <translation>レンジ選択</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2397"/>
        <location filename="MainWindow.ui" line="2795"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Get the selected range of the selected sensor.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>指定した計測器の現在のレンジ取得</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2403"/>
        <source>Get Range</source>
        <translation>レンジ取得</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2410"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Get the selected ranges of all of the sensors.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;レンジ選択のある全計測器の現在のレンジを取得する。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2416"/>
        <source>Get All Range</source>
        <translation>全レンジ取得</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4005"/>
        <source>Set up SSD</source>
        <translation>SSD設定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4791"/>
        <source>At Cursor</source>
        <translation>カーソル点</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5155"/>
        <source>Log  </source>
        <translation>Log </translation>
    </message>
    <message>
        <source>eV  Gain</source>
        <translation type="obsolete">eV  ゲイン</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4641"/>
        <source>Clear</source>
        <translation>クリア</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4681"/>
        <source>Live</source>
        <translation>ライブ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4774"/>
        <source>Real</source>
        <translation>リアル</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4784"/>
        <source>Preset:</source>
        <translation>プリセット</translation>
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
        <location filename="MainWindow.ui" line="4852"/>
        <source>u-sec</source>
        <translation>μs</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4859"/>
        <source>eV</source>
        <translation>eV</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4872"/>
        <source>mV/keV</source>
        <translation>mV/keV</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4885"/>
        <source>ROI  Start</source>
        <translation>積分始点</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4898"/>
        <source>Peak. T.</source>
        <translation>ピーキング時間</translation>
    </message>
    <message>
        <source>eV  Carib.</source>
        <translation type="obsolete">eV  較正</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4919"/>
        <source>sec</source>
        <translation>秒</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4926"/>
        <source>Thresh.</source>
        <translation>閾値</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4946"/>
        <source>Dyn. Range</source>
        <translation>ダイナミックレンジ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4959"/>
        <source>SSD No.</source>
        <translation>SSD チャンネル</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1264"/>
        <location filename="MainWindow.ui" line="4260"/>
        <location filename="MainWindow.ui" line="5344"/>
        <source>8</source>
        <translation>8</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1131"/>
        <location filename="MainWindow.ui" line="4282"/>
        <location filename="MainWindow.ui" line="5360"/>
        <source>9</source>
        <translation>9</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1283"/>
        <source>SetUp BLKs</source>
        <translation>測定ブロック設定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1553"/>
        <source>Select Detectors</source>
        <translation>検出器選択</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1876"/>
        <source>Start Meas.</source>
        <translation>測定開始</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5005"/>
        <source>Set Up a SSD Channel</source>
        <translation>SSDの各チャンネルの設定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4301"/>
        <location filename="MainWindow.ui" line="5373"/>
        <location filename="MainWindow.ui" line="6043"/>
        <source>10</source>
        <translation>10</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4320"/>
        <location filename="MainWindow.ui" line="5386"/>
        <location filename="MainWindow.ui" line="6024"/>
        <source>11</source>
        <translation>11</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4339"/>
        <location filename="MainWindow.ui" line="5399"/>
        <location filename="MainWindow.ui" line="6005"/>
        <source>12</source>
        <translation>12</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4358"/>
        <location filename="MainWindow.ui" line="5412"/>
        <location filename="MainWindow.ui" line="6435"/>
        <source>13</source>
        <translation>13</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4377"/>
        <location filename="MainWindow.ui" line="5425"/>
        <location filename="MainWindow.ui" line="6378"/>
        <source>14</source>
        <translation>14</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4396"/>
        <location filename="MainWindow.ui" line="5438"/>
        <location filename="MainWindow.ui" line="6359"/>
        <source>15</source>
        <translation>15</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4415"/>
        <location filename="MainWindow.ui" line="5451"/>
        <location filename="MainWindow.ui" line="6416"/>
        <source>16</source>
        <translation>16</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4434"/>
        <location filename="MainWindow.ui" line="5492"/>
        <location filename="MainWindow.ui" line="6397"/>
        <source>17</source>
        <translation>17</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4453"/>
        <location filename="MainWindow.ui" line="5505"/>
        <location filename="MainWindow.ui" line="6340"/>
        <source>18</source>
        <translation>18</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4067"/>
        <source>Select a SSD ch (MCA spectrum)</source>
        <oldsource>Select a SSD ch</oldsource>
        <translation>SSD選択(MCAスペクトル)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5557"/>
        <source>Select SSD channels (XAFS meas.)</source>
        <oldsource>Select SSD channels</oldsource>
        <translation>複数SSD選択(積算/本測定対象)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1628"/>
        <source>I                 </source>
        <translation></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2801"/>
        <source>Set Offset</source>
        <translation>設定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2823"/>
        <source>Check/Set Offset(Dark)</source>
        <translation>バックグラウンド確認／設定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2426"/>
        <source>Auto Rang</source>
        <translation>オートレンジ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3318"/>
        <source>Move a motor or scan by moving a motor while monitoring a sensor.</source>
        <translation>駆動軸を動かす、もしくは軸を動かしながらスキャンを行う。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2910"/>
        <source>Move the monochromator where you want.</source>
        <translation>分光器の角度設定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3052"/>
        <source>Select an unit for all.</source>
        <translation>全部の単位を一度に設定します。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2423"/>
        <source>When the sensor can be used in auto-range mode, the check button is active.</source>
        <translation>選択した検出器でオートレンジモードが使える場合、このチェックボタンが有効になります。</translation>
    </message>
    <message>
        <source>Make info File</source>
        <translation type="obsolete">Info ファイル作成</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1407"/>
        <source>Current Pos.</source>
        <translation>現在位置</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1414"/>
        <source>Rpt.</source>
        <translation>回目</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1440"/>
        <source>Point</source>
        <translation>点目</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2617"/>
        <source>Record the data during the monitor.</source>
        <translation>モニータ時のデータを記録します。(モニタ開始前にチェック)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2620"/>
        <source>Record</source>
        <translation>記録</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2662"/>
        <source>Save the monitored data after the monitoring.</source>
        <translation>(モニタ後)モニタしたデータを記録します。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5097"/>
        <source>Disp. Elm. Names</source>
        <translation>元素名表示</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5113"/>
        <source>Select Elms.</source>
        <translation>表示元素選択</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5120"/>
        <source>Show Always</source>
        <translation>選択した元素を常に表示</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5171"/>
        <source>eV        Gain</source>
        <translation>eV       ゲイン</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4826"/>
        <source>      ROI end</source>
        <oldsource>       ROI end</oldsource>
        <translation>     積分終点</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4839"/>
        <source>         In ROI</source>
        <oldsource>          In ROI</oldsource>
        <translation>       積分値</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4905"/>
        <source>eV       Carib.</source>
        <translation>eV        較正.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5015"/>
        <source>Set All</source>
        <translation>Set All</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5683"/>
        <source>Read Data</source>
        <translation>データ読込</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5856"/>
        <source>Close View</source>
        <translation>View を閉じる</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5886"/>
        <source>Log / Record</source>
        <translation>ログ/記録</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5900"/>
        <source>Log File Name:</source>
        <translation>ログファイル名</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5923"/>
        <source>New</source>
        <translation>選択</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5949"/>
        <source>Add Log Comment:</source>
        <translation>ログコメント追加</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5867"/>
        <source>Stat/Config</source>
        <translation>状態/設定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6493"/>
        <source>&amp;File</source>
        <translation>ファイル(&amp;F)</translation>
    </message>
    <message>
        <source>&amp;Config</source>
        <translation type="obsolete">設定(&amp;C)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6502"/>
        <source>&amp;Quit</source>
        <translation>終了(&amp;Q)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6507"/>
        <source>Japanese</source>
        <translation>日本語</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6512"/>
        <source>English</source>
        <translation>English</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6517"/>
        <source>Set &amp;Lattice Const. Monochro Crystal</source>
        <translation>結晶・格子定数設定(&amp;L)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6522"/>
        <source>Set &amp;Stars Server Address</source>
        <translation>Starsサーバ設定(&amp;S)</translation>
    </message>
    <message>
        <location filename="Goto.cpp" line="9"/>
        <location filename="Goto.cpp" line="11"/>
        <source>Setup: %1 : Reached at %2</source>
        <oldsource>Setup: %1 : Reached at %2
</oldsource>
        <translation>設定: %1 : %2 に到達</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="25"/>
        <source>The name is new, but the data is old.</source>
        <translation>ファイル名は新規に選択されたものですが、データはすでに記録済みのデータです。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="26"/>
        <source>The data is new, but the name is old.</source>
        <translation>データは新しく測定されたものですが、ファイル名は古いままです。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="27"/>
        <source>The data and the name is new, but not saved.</source>
        <translation>データもファイル名も新しいものですが保存されていません。</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="95"/>
        <source>Mono: %1 (%2 A)</source>
        <oldsource>Mono: %1 (%2 A)
</oldsource>
        <translation>分光結晶: %1 (%2 A)</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="259"/>
        <source>Encorder is set from %1 to %2</source>
        <translation>エンコーダの値を %1 から %2 にセットしました</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="321"/>
        <source>Current Position [%1] deg [%2] keV</source>
        <translation>現在位置 [%1] deg [%2] keV</translation>
    </message>
    <message>
        <source>Current Position [%1] keV</source>
        <oldsource>Current Position [%1] keV
</oldsource>
        <translation type="obsolete">分光器現在位置 [%1] keV</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="376"/>
        <source>No Scree is available!</source>
        <translation>使用可能な描画領域(View)がありません</translation>
    </message>
    <message>
        <location filename="Measurement.cpp" line="41"/>
        <source>Start Measurement!</source>
        <translation>測定開始</translation>
    </message>
    <message>
        <location filename="Measurement.cpp" line="53"/>
        <source>Writing Header.</source>
        <translation>ヘッダ記録</translation>
    </message>
    <message>
        <location filename="Measurement.cpp" line="109"/>
        <source>Meas: Repeat %1</source>
        <oldsource>Meas: Repeat %1
</oldsource>
        <translation>測定： %1回目</translation>
    </message>
    <message>
        <location filename="Measurement.cpp" line="117"/>
        <location filename="QXafs.cpp" line="185"/>
        <source>The Measurement has Finished</source>
        <translation>測定終了</translation>
    </message>
    <message>
        <location filename="Measurement.cpp" line="118"/>
        <source>Meas: Finished</source>
        <oldsource>Meas: Finished
</oldsource>
        <translation>測定: 終了</translation>
    </message>
    <message>
        <location filename="Measurement.cpp" line="178"/>
        <location filename="MwMeas.cpp" line="936"/>
        <source>mu</source>
        <translation>μt</translation>
    </message>
    <message>
        <location filename="MwCommon.cpp" line="146"/>
        <location filename="MwCommon.cpp" line="158"/>
        <source>Absorption edge of the atom is out of range.</source>
        <translation>対象原子の吸収端が測定可能範囲外です。</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="49"/>
        <source>Return</source>
        <translation>原位置復帰</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="50"/>
        <source>Stay</source>
        <translation>その場に停止</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="62"/>
        <location filename="MwMeas.cpp" line="69"/>
        <location filename="MwMeas.cpp" line="75"/>
        <source>Cancel</source>
        <translation>キャンセル</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="63"/>
        <location filename="MwMeas.cpp" line="70"/>
        <location filename="MwMeas.cpp" line="76"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="64"/>
        <source>&lt;h1&gt;&lt;center&gt;Stop ?&lt;/center&gt;&lt;/h1&gt;</source>
        <translation>終了?</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="65"/>
        <source>Stop or Continue</source>
        <translation>停止／再開</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="71"/>
        <source>Over Write ?</source>
        <translation>上書きしますか?</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="77"/>
        <source>Have you seleced ?</source>
        <translation>選択済み？</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="622"/>
        <source>Points: </source>
        <translation>点数: </translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="633"/>
        <source>Time: </source>
        <translation>時間: </translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="779"/>
        <source>I0</source>
        <translation>I0</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="780"/>
        <source>mu(E)</source>
        <translation>μ(E)</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="781"/>
        <source>Energy</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="782"/>
        <source>[keV]</source>
        <translation>[keV]</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="854"/>
        <source>Monochro is moving!</source>
        <translation>分光器回転中</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="858"/>
        <location filename="MwMeas.cpp" line="971"/>
        <source>Meas cannot Start : (%1) is disabled</source>
        <translation>(%1) が使用できないため測定を開始できません。</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="879"/>
        <source>Meas cannot Start : in MCA measurement</source>
        <translation>MCA 測定中のため XAFS 測定を開始できません</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="887"/>
        <source>I1 must be selected for QXAFS</source>
        <translation>Q-XAFS では、I1 は必ず選択する必要があります。</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="891"/>
        <source>19ch SSD can not be used for QXAFS</source>
        <translation>Q-XAFS では 19素子SSDは使えません。</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="896"/>
        <source>Aux1 and 2 can not be used for QXAFS</source>
        <translation>Q-XAFS では Aux1 および 2 は使用できません。</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="901"/>
        <source>Selected I0 Sensor can not be used for QXAFS</source>
        <translation>I0としえて選択した検出器は Q-XAFS には使用できません。</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="906"/>
        <source>Selected I1 Sensor can not be used for QXAFS</source>
        <translation>I1としえて選択した検出器は Q-XAFS には使用できません。</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="1046"/>
        <source>Meas: Start %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</source>
        <translation>測定開始 %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="1084"/>
        <source>Meas: Break %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</source>
        <translation>測定中断 %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="1107"/>
        <source>Meas: Stopped %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</source>
        <translation>測定中断 : %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="1129"/>
        <source>Meas: Pausing %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</source>
        <translation>測定中断中 %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="1136"/>
        <location filename="MwMeas.cpp" line="1182"/>
        <source>Meas: Resume %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</source>
        <translation>測定再開 %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="1173"/>
        <source>Meas: Pause %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</source>
        <translation>測定中断 %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</translation>
    </message>
    <message>
        <location filename="Dark.cpp" line="11"/>
        <source>BG Measuring</source>
        <translation>BG 計測中</translation>
    </message>
    <message>
        <location filename="Dark.cpp" line="18"/>
        <source>Already Measuring Dark!</source>
        <translation>バックグラウンド計測中!</translation>
    </message>
    <message>
        <location filename="Dark.cpp" line="23"/>
        <source>Measurement is going on!</source>
        <translation>測定中!</translation>
    </message>
    <message>
        <location filename="Dark.cpp" line="39"/>
        <source>MCA measurement is going on</source>
        <translation>MCA測定中</translation>
    </message>
    <message>
        <location filename="Dark.cpp" line="56"/>
        <location filename="MwSetup.cpp" line="576"/>
        <location filename="MwSetup.cpp" line="584"/>
        <location filename="MwSetup.cpp" line="668"/>
        <location filename="MwSetup.cpp" line="717"/>
        <location filename="MwSetup.cpp" line="728"/>
        <source>Scan cannot Start : (%1) is disabled</source>
        <translation>%1 が disable なので、測定開始できません。</translation>
    </message>
    <message>
        <location filename="Dark.cpp" line="83"/>
        <source>Close Shutter!!</source>
        <translation>シャッター CLOSE 確認</translation>
    </message>
    <message>
        <location filename="Dark.cpp" line="86"/>
        <source>Make sure that shutte is closed.  Then push the &apos;red&apos; button.</source>
        <translation>シャッターが閉まっていることを確認し、赤くなっている「シャッター CLOSE 確認」ボタンを押して下さい。</translation>
    </message>
    <message>
        <location filename="Dark.cpp" line="141"/>
        <source>Invalid dwell time [%1] was set for [%2].However, the background was set as if the time was set at 1sec.</source>
        <translation>計測器[%2]に指定された計測時間[%1]が正しくありません。1秒に設定して計測します。</translation>
    </message>
    <message>
        <location filename="Dark.cpp" line="152"/>
        <source>Open Shutter!!</source>
        <translation>シャッター OPEN 確認</translation>
    </message>
    <message>
        <location filename="Dark.cpp" line="154"/>
        <source>Make sure that shutte is opened.  Then push the &apos;red&apos; button.</source>
        <translation>シャッタがーが開いていることを確認し、赤くなっている「シャッター OPEN 確認」ボタンを押して下さい。</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="866"/>
        <source>Invalid block data.</source>
        <translation>測定ブロックの指定が誤っています。</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="870"/>
        <source>Data File is not Selected!</source>
        <translation>データファイルが選択されていません。</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="874"/>
        <source>Detectors are not selected properly!</source>
        <translation>測定のための検出器の指定が正しくありません。</translation>
    </message>
    <message>
        <location filename="Dark.cpp" line="69"/>
        <location filename="MwMeas.cpp" line="991"/>
        <source>Selected sensors [%1] and [%2] are conflicting.</source>
        <translation>選択した[%1]と[%2]は同時に使えません。</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="1006"/>
        <source>The Sensor(s)%1 should be range selected.
Have you selected the range in &apos;Setup Condition&apos;</source>
        <translation>選択した計測器 %1 について、「設定」の画面でレンジを設定しましたか？</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="1038"/>
        <source>File [%1] Over Write ?</source>
        <translation>ファイル [%1] を上書きして宜しいですか？</translation>
    </message>
    <message>
        <source>&lt;h1&gt;&lt;center&gt;File [%1] Over Write ?&lt;/center&gt;&lt;/h1&gt;</source>
        <translation type="obsolete">&lt;h1&gt;&lt;center&gt;ファイル [%1] に上書きして宜しいですか?&lt;/center&gt;&lt;/h1&gt;</translation>
    </message>
    <message>
        <source>Meas: Start (%1 keV)</source>
        <oldsource>Meas: Start (%1 keV)
</oldsource>
        <translation type="obsolete">測定: 開始(%1 keV)</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="1053"/>
        <location filename="MwSSDSetUp.cpp" line="429"/>
        <location filename="MwSetup.cpp" line="497"/>
        <location filename="MwSetup.cpp" line="616"/>
        <location filename="MwSetup.cpp" line="766"/>
        <source>Stop</source>
        <translation>中止</translation>
    </message>
    <message>
        <source>Meas: Break (%1 keV)</source>
        <oldsource>Meas: Break (%1 keV)
</oldsource>
        <translation type="obsolete">測定: 中断(%1 keV)</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="1090"/>
        <location filename="MwMeas.cpp" line="1179"/>
        <source>Resume</source>
        <translation>再開</translation>
    </message>
    <message>
        <source>Meas: Stopped (%1 keV)</source>
        <oldsource>Meas: Stopped (%1 keV)
</oldsource>
        <translation type="obsolete">測定: 停止 (%1 keV)</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="1112"/>
        <source>The Measurement is Stopped</source>
        <translation>測定が中止されました。</translation>
    </message>
    <message>
        <source>Meas: Pausing (%1 keV)</source>
        <oldsource>Meas: Pausing (%1 keV)
</oldsource>
        <translation type="obsolete">測定: ポーズ (%1 keV)</translation>
    </message>
    <message>
        <source>Measu: Resume (%1 keV)</source>
        <oldsource>Measu: Resume (%1 keV)
</oldsource>
        <translation type="obsolete">測定: 再開 (%1 keV)</translation>
    </message>
    <message>
        <source>Meas: Pause (%1 keV)</source>
        <oldsource>Meas: Pause (%1 keV)
</oldsource>
        <translation type="obsolete">測定: ポーズ (%1 keV)</translation>
    </message>
    <message>
        <source>Meas: Resume (%1 keV)</source>
        <oldsource>Meas: Resume (%1 keV)
</oldsource>
        <translation type="obsolete">測定: 再開 (%1 keV)</translation>
    </message>
    <message>
        <location filename="MwSSDSetUp.cpp" line="20"/>
        <location filename="MwSSDSetUp.cpp" line="26"/>
        <location filename="MwSSDSetUp.cpp" line="277"/>
        <location filename="MwSSDSetUp.cpp" line="296"/>
        <source>Active</source>
        <translation>アクティブ</translation>
    </message>
    <message>
        <location filename="MwSSDSetUp.cpp" line="178"/>
        <source>MCA data is not valid</source>
        <translation>正しいMCAデータがありません。</translation>
    </message>
    <message>
        <location filename="MwSSDSetUp.cpp" line="182"/>
        <location filename="MwSetup.cpp" line="245"/>
        <location filename="MwSetup.cpp" line="806"/>
        <source>Save file name is not selected</source>
        <translation>ファイル名が選択されていません。</translation>
    </message>
    <message>
        <location filename="MwSSDSetUp.cpp" line="202"/>
        <location filename="MwSetup.cpp" line="251"/>
        <location filename="MwSetup.cpp" line="692"/>
        <location filename="MwSetup.cpp" line="812"/>
        <source>The file [%1] can not open to record the data</source>
        <translation>記録のためのファイル [%1]が開けません。</translation>
    </message>
    <message>
        <location filename="MwSSDSetUp.cpp" line="282"/>
        <location filename="MwSSDSetUp.cpp" line="302"/>
        <source>Inactive</source>
        <translation>インアクティブ</translation>
    </message>
    <message>
        <location filename="MwSSDSetUp.cpp" line="344"/>
        <location filename="MwSSDSetUp.cpp" line="359"/>
        <location filename="MwSSDSetUp.cpp" line="498"/>
        <source>ROI cannot change while the XAFS measurements</source>
        <translation>XAFS測定中はROIを変更できません</translation>
    </message>
    <message>
        <location filename="MwSSDSetUp.cpp" line="423"/>
        <source>Scan cannot Start : SSD is disabled</source>
        <translation>SSD が disable なので測定が開始できません。</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="241"/>
        <source>Scan data is not valid</source>
        <translation>正しいスキャンデータがありません。</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="395"/>
        <source>Current Position of [%1] : [%2] %3</source>
        <oldsource>Current Position of [%1] : [%2] %3
</oldsource>
        <translation>[%1]の現在位置 : [%2] %3</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="508"/>
        <location filename="MwSetup.cpp" line="512"/>
        <source>Setup: %1 : GoTo %2 : Speed %3</source>
        <oldsource>Setup: %1 : GoTo %2 : Speed %3
</oldsource>
        <translation>設定: %1 をスピード %3 で %2 に移動</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="535"/>
        <location filename="MwSetup.cpp" line="538"/>
        <source>Setup: %1 : Stopped at %2</source>
        <oldsource>Setup: %1 : Stopped at %2
</oldsource>
        <translation>設定: %1 を %2 で停止</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="560"/>
        <source>No drawing screen is available</source>
        <translation>使用可能な描画領域(View)がありません。</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="605"/>
        <source>Error: Scan Step is 0.</source>
        <translation>エラー: スキャンステップが 0 です。</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="610"/>
        <source>Scan Start (%1 %2)</source>
        <oldsource>Scan Start (%1 %2)
</oldsource>
        <translation>スキャン: 開始 (%1, %2)</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="675"/>
        <source>No drawing area is avairable</source>
        <translation>使用可能な描画領域(View)がありません。</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="681"/>
        <source>No Record file is selected</source>
        <translation>記録ファイルが選択されていません。</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="743"/>
        <location filename="MwSetup.cpp" line="828"/>
        <source>	%1[%2]</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="802"/>
        <source>Monitor data is not valid</source>
        <translation>有効なデータがありません。</translation>
    </message>
    <message>
        <location filename="ReadDef.cpp" line="15"/>
        <source>File [%1] is selected as XafsM2 definition file.</source>
        <translation>ファイル[%1]が XafsM2 の定義ファイルに選択されました。</translation>
    </message>
    <message>
        <location filename="ReadDef.cpp" line="18"/>
        <source>FilePath = [%1]</source>
        <translation>FilePath = [%1]</translation>
    </message>
    <message>
        <location filename="ReadDef.cpp" line="22"/>
        <source>The file [%1] is not. Use default parameters.</source>
        <translation>The file [%1] is not. Use default parameters.</translation>
    </message>
    <message>
        <location filename="ReadDef.cpp" line="26"/>
        <source>Error cannot open [%1].</source>
        <translation>Error cannot open [%1].</translation>
    </message>
    <message>
        <location filename="ReadDef.cpp" line="79"/>
        <location filename="ReadDef.cpp" line="104"/>
        <source>::Undefined Unit type [%1]</source>
        <translation>::Undefined Unit type [%1]</translation>
    </message>
    <message>
        <location filename="ReadDef.cpp" line="162"/>
        <source>Undefined Key word [%1]</source>
        <translation>Undefined Key word [%1]</translation>
    </message>
    <message>
        <location filename="ReadDef.cpp" line="243"/>
        <source>UIDs [%1](at line %2) and [%3](at line %4) are duplicated.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="ReadDef.cpp" line="246"/>
        <source>1st one is : Type[%1] Identifier[%2] Driver[%3] Node[%4]</source>
        <translation></translation>
    </message>
    <message>
        <location filename="ReadDef.cpp" line="248"/>
        <source>2nd one is : Type[%1] Identifier[%2] Driver[%3] Node[%4]</source>
        <translation></translation>
    </message>
    <message>
        <location filename="Scan.cpp" line="21"/>
        <source>Going to initial position.</source>
        <translation>開始点に移動中</translation>
    </message>
    <message>
        <location filename="Scan.cpp" line="82"/>
        <source>The Scan has Finished</source>
        <translation>スキャン終了</translation>
    </message>
    <message>
        <location filename="Scan.cpp" line="83"/>
        <source>Scan Finished</source>
        <oldsource>Scan Finished
</oldsource>
        <translation>スキャン終了</translation>
    </message>
    <message>
        <location filename="MwLog.cpp" line="33"/>
        <source>Cannot open Log File [%1]
</source>
        <translation>ログファイル[%1]が開けません。</translation>
    </message>
    <message>
        <location filename="QXafs.cpp" line="135"/>
        <source>Start QXAFS Measurement!</source>
        <translation>Q-XAFS 測定開始</translation>
    </message>
    <message>
        <location filename="QXafs.cpp" line="186"/>
        <source>Meas: QXafs Finished</source>
        <translation>Q-XAFS 測定終了</translation>
    </message>
</context>
<context>
    <name>PeriodicT</name>
    <message>
        <location filename="PeriodicT.ui" line="14"/>
        <source>Periodic Table</source>
        <translation>周期表</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="49"/>
        <source>H</source>
        <translation>H</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="62"/>
        <source>He</source>
        <translation>He</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="75"/>
        <source>Li</source>
        <translation>Li</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="88"/>
        <source>Be</source>
        <translation>Be</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="101"/>
        <source>B</source>
        <translation>B</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="114"/>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="127"/>
        <source>N</source>
        <translation>N</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="140"/>
        <source>O</source>
        <translation>O</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="153"/>
        <source>F</source>
        <translation>F</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="166"/>
        <source>Ne</source>
        <translation>Ne</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="179"/>
        <source>Na</source>
        <translation>Na</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="192"/>
        <source>Mg</source>
        <translation>Mg</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="205"/>
        <source>Al</source>
        <translation>Al</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="218"/>
        <source>Si</source>
        <translation>Si</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="231"/>
        <source>P</source>
        <translation>P</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="244"/>
        <source>S</source>
        <translation>S</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="257"/>
        <source>Cl</source>
        <translation>Cl</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="270"/>
        <source>Ar</source>
        <translation>Ar</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="283"/>
        <source>K</source>
        <translation>K</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="296"/>
        <source>Ca</source>
        <translation>Ca</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="309"/>
        <source>Sc</source>
        <translation>Sc</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="322"/>
        <source>Ti</source>
        <translation>Ti</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="338"/>
        <source>V</source>
        <translation>V</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="354"/>
        <source>Cr</source>
        <translation>Cr</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="370"/>
        <source>Mn</source>
        <translation>Mn</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="386"/>
        <source>Fe</source>
        <translation>Fe</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="402"/>
        <source>Co</source>
        <translation>Co</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="418"/>
        <source>Ni</source>
        <translation>Ni</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="434"/>
        <source>Cu</source>
        <translation>Cu</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="450"/>
        <source>Zn</source>
        <translation>Zn</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="466"/>
        <source>Ga</source>
        <translation>Ga</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="482"/>
        <source>Ge</source>
        <translation>Ge</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="498"/>
        <source>As</source>
        <translation>As</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="514"/>
        <source>Se</source>
        <translation>Se</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="530"/>
        <source>Br</source>
        <translation>Br</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="546"/>
        <source>Kr</source>
        <translation>Kr</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="562"/>
        <source>Rb</source>
        <translation>Rb</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="578"/>
        <source>Sr</source>
        <translation>Sr</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="594"/>
        <source>Y</source>
        <translation>Y</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="610"/>
        <source>Zr</source>
        <translation>Zr</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="626"/>
        <source>Nb</source>
        <translation>Nb</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="642"/>
        <source>Mo</source>
        <translation>Mo</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="655"/>
        <source>Tc</source>
        <translation>Tc</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="668"/>
        <source>Ru</source>
        <translation>Ru</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="681"/>
        <source>Rh</source>
        <translation>Rh</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="694"/>
        <source>Pd</source>
        <translation>Pb</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="707"/>
        <source>Ag</source>
        <translation>Ag</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="720"/>
        <source>Cd</source>
        <translation>Cd</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="733"/>
        <source>In</source>
        <translation>In</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="746"/>
        <source>Sn</source>
        <translation>Sn</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="759"/>
        <source>Sb</source>
        <translation>Sb</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="772"/>
        <source>Te</source>
        <translation>Te</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="785"/>
        <source>I</source>
        <translation>I</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="798"/>
        <source>Xe</source>
        <translation>Xe</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="814"/>
        <source>Cs</source>
        <translation>Cs</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="830"/>
        <source>Ba</source>
        <translation>Ba</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="846"/>
        <location filename="PeriodicT.ui" line="1313"/>
        <source>*1</source>
        <translation>*1</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="862"/>
        <source>Hf</source>
        <translation>Hf</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="878"/>
        <source>Ta</source>
        <translation>Ta</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="894"/>
        <source>W</source>
        <translation>W</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="910"/>
        <source>Re</source>
        <translation>Re</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="926"/>
        <source>Os</source>
        <translation>Os</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="943"/>
        <source>Ir</source>
        <translation>Ir</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="959"/>
        <source>Pt</source>
        <translation>Pt</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="975"/>
        <source>Au</source>
        <translation>Au</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="991"/>
        <source>Hg</source>
        <translation>Hg</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1007"/>
        <source>Tl</source>
        <translation>Tl</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1023"/>
        <source>Pb</source>
        <translation>Pb</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1039"/>
        <source>Bi</source>
        <translation>Bi</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1055"/>
        <source>Po</source>
        <translation>Po</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1071"/>
        <source>At</source>
        <translation>At</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1087"/>
        <source>Rn</source>
        <translation>Rn</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1103"/>
        <source>Fr</source>
        <translation>Fr</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1119"/>
        <source>Ra</source>
        <translation>Ra</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1135"/>
        <location filename="PeriodicT.ui" line="1560"/>
        <source>*2</source>
        <translation>*2</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1151"/>
        <source>Rf</source>
        <translation>Rf</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1167"/>
        <source>Db</source>
        <translation>Db</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1183"/>
        <source>Sg</source>
        <translation>Sg</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1199"/>
        <source>Bh</source>
        <translation>Bh</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1215"/>
        <source>Hs</source>
        <translation>Hs</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1231"/>
        <source>Mt</source>
        <translation>Mt</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1247"/>
        <source>Ds</source>
        <translation>Ds</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1263"/>
        <source>Rg</source>
        <translation>Rg</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1279"/>
        <source>Cn</source>
        <translation>Cn</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1329"/>
        <source>La</source>
        <translation>La</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1345"/>
        <source>Ce</source>
        <translation>Ce</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1361"/>
        <source>Pr</source>
        <translation>Pr</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1377"/>
        <source>Nd</source>
        <translation>Nd</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1393"/>
        <source>Pm</source>
        <translation>Pm</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1409"/>
        <source>Sm</source>
        <translation>Sm</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1425"/>
        <source>Eu</source>
        <translation>Eu</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1441"/>
        <source>Gd</source>
        <translation>Gd</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1457"/>
        <source>Tb</source>
        <translation>Tb</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1473"/>
        <source>Dy</source>
        <translation>Dy</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1489"/>
        <source>Ho</source>
        <translation>Ho</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1505"/>
        <source>Er</source>
        <translation>Er</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1521"/>
        <source>Tm</source>
        <translation>Tm</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1537"/>
        <source>Yb</source>
        <translation>Yb</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1553"/>
        <source>Lu</source>
        <translation>Lu</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1576"/>
        <source>Ac</source>
        <translation>Ac</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1592"/>
        <source>Th</source>
        <translation>Th</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1608"/>
        <source>Pa</source>
        <translation>Pa</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1624"/>
        <source>U</source>
        <translation>U</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1637"/>
        <source>Np</source>
        <translation>Np</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1650"/>
        <source>Pu</source>
        <translation>Pu</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1663"/>
        <source>Am</source>
        <translation>Am</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1676"/>
        <source>Cm</source>
        <translation>Cm</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1689"/>
        <source>Bk</source>
        <translation>Bk</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1702"/>
        <source>Cf</source>
        <translation>Cf</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1715"/>
        <source>Es</source>
        <translation>Es</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1728"/>
        <source>Fm</source>
        <translation>Fm</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1741"/>
        <source>Md</source>
        <translation>Md</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1754"/>
        <source>No</source>
        <translation>No</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1767"/>
        <source>Lr</source>
        <translation>Lr</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1799"/>
        <source>All</source>
        <translation>全選択</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1809"/>
        <source>None</source>
        <translation>全解除</translation>
    </message>
    <message>
        <location filename="PeriodicT.ui" line="1832"/>
        <source>Close</source>
        <translation>閉じる</translation>
    </message>
</context>
<context>
    <name>ScrollWGrid</name>
    <message>
        <location filename="ScrollWGrid.ui" line="14"/>
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
        <oldsource>Mono: Cnaged to %1 (%2 A)
</oldsource>
        <translation type="obsolete">分光結晶: 変更 %1 (%2 A)</translation>
    </message>
</context>
<context>
    <name>SelMC2</name>
    <message>
        <location filename="SelMC2.ui" line="20"/>
        <source>Frame</source>
        <translation></translation>
    </message>
    <message>
        <location filename="SelMC2.ui" line="101"/>
        <source>Change</source>
        <translation>変更</translation>
    </message>
    <message>
        <location filename="SelMC2.ui" line="76"/>
        <source>Monochro Crystal</source>
        <translation>分光結晶</translation>
    </message>
    <message>
        <location filename="SelMC2.ui" line="60"/>
        <source>3.1355316 A</source>
        <translation>3.1355316 A</translation>
    </message>
    <message>
        <location filename="SelMC2.cpp" line="18"/>
        <location filename="SelMC2.cpp" line="42"/>
        <source>%1 A</source>
        <translation>%1 A</translation>
    </message>
    <message>
        <location filename="SelMC2.cpp" line="22"/>
        <source>Cancel</source>
        <translation>Cancel</translation>
    </message>
    <message>
        <location filename="SelMC2.cpp" line="23"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="SelMC2.cpp" line="24"/>
        <source>&lt;h1&gt;&lt;center&gt;Do you really change it ?&lt;/center&gt;&lt;/h1&gt;</source>
        <translation>&lt;h1&gt;&lt;center&gt;本当に変更しますか?&lt;/center&gt;&lt;/h1&gt;</translation>
    </message>
    <message>
        <location filename="SelMC2.cpp" line="25"/>
        <source>Really Change?</source>
        <translation>本当に変更しますか?</translation>
    </message>
    <message>
        <location filename="SelMC2.cpp" line="44"/>
        <source>Mono: Cnaged to %1 (%2 A)</source>
        <translation>分光結晶: 変更 %1 (%2 A)</translation>
    </message>
</context>
<context>
    <name>Stars</name>
    <message>
        <location filename="Stars.cpp" line="74"/>
        <source>Cannot open [%1.key]</source>
        <translation>Cannot open [%1.key]</translation>
    </message>
    <message>
        <location filename="Stars.cpp" line="81"/>
        <source>Cannot open both [%1.key] and [:%2.key]</source>
        <translation>Cannot open both [%1.key] and [:%2.key]</translation>
    </message>
    <message>
        <location filename="Stars.cpp" line="147"/>
        <source>Connecting Stars Server [%1] [%2] as [%3]</source>
        <translation>Starsサーバ [%1][%2] に [%3] として接続できませんでした。</translation>
    </message>
    <message>
        <location filename="Stars.cpp" line="168"/>
        <source>%1 %2
</source>
        <translation></translation>
    </message>
    <message>
        <location filename="Stars.cpp" line="181"/>
        <source>Success to connect.</source>
        <translation>Stars サーバとの接続成功</translation>
    </message>
    <message>
        <location filename="Stars.cpp" line="194"/>
        <source>Receive an answer from Stars [%1]</source>
        <translation>Stars サーバからのメッセージ [%1]</translation>
    </message>
    <message>
        <location filename="Stars.cpp" line="318"/>
        <source>Receive an event from Stars [%1]</source>
        <translation>Stars サーバからのイベント [%1]</translation>
    </message>
    <message>
        <location filename="Stars.cpp" line="335"/>
        <source>Receive an unricognized message from Stars [%1]</source>
        <translation>Stars サーバからの解釈できないメッセージ [%1]</translation>
    </message>
    <message>
        <location filename="Stars.cpp" line="337"/>
        <source>Others:  [%1] -&gt; [%2] : [%3] [%4]
</source>
        <translation>Others:  [%1] -&gt; [%2] : [%3] [%4</translation>
    </message>
    <message>
        <location filename="Stars.cpp" line="354"/>
        <source>Sending a message [%1] to Stars</source>
        <translation>Stars サーバにメッセージ送信: [%1]</translation>
    </message>
</context>
<context>
    <name>StarsSV</name>
    <message>
        <location filename="StarsSV.ui" line="14"/>
        <source>Set Stars Server</source>
        <translation>Stars サーバ情報設定</translation>
    </message>
    <message>
        <location filename="StarsSV.ui" line="29"/>
        <source>Stars Server Address: </source>
        <translation>Starsサーバアドレス: </translation>
    </message>
    <message>
        <location filename="StarsSV.ui" line="52"/>
        <source>Port No.:</source>
        <translation>ポート番号: </translation>
    </message>
    <message>
        <location filename="StarsSV.ui" line="86"/>
        <source>ReConnect</source>
        <translation>再接続</translation>
    </message>
</context>
<context>
    <name>StarsSV2</name>
    <message>
        <location filename="StarsSV2.ui" line="14"/>
        <source>Frame</source>
        <translation>Frame</translation>
    </message>
    <message>
        <location filename="StarsSV2.ui" line="50"/>
        <source>Stars Server Address </source>
        <translation>Starsサーバアドレス</translation>
    </message>
    <message>
        <location filename="StarsSV2.ui" line="94"/>
        <source>Port No.</source>
        <translation>ポート番号</translation>
    </message>
    <message>
        <location filename="StarsSV2.ui" line="160"/>
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
        <location filename="Status.cpp" line="58"/>
        <source>: Drivers status watch active</source>
        <translation>ドライバ状態監視</translation>
    </message>
    <message>
        <location filename="Status.cpp" line="67"/>
        <source>Drivers</source>
        <translation>ドライバ</translation>
    </message>
    <message>
        <location filename="Status.cpp" line="68"/>
        <source>Devices</source>
        <translation>デバイス</translation>
    </message>
    <message>
        <location filename="Status.cpp" line="69"/>
        <source>Enable</source>
        <translation>Enable</translation>
    </message>
    <message>
        <location filename="Status.cpp" line="70"/>
        <source>Clr. Enable</source>
        <translation>Clr. Enable</translation>
    </message>
    <message>
        <location filename="Status.cpp" line="75"/>
        <source>Clr. Busys</source>
        <translation>Clr. Busys</translation>
    </message>
    <message>
        <source>Clear Enable</source>
        <translation type="obsolete">Clear Enable</translation>
    </message>
    <message>
        <location filename="Status.cpp" line="71"/>
        <source>IsBusy</source>
        <translation>IsBusy</translation>
    </message>
    <message>
        <location filename="Status.cpp" line="72"/>
        <source>Busy Units</source>
        <translation>Busy Units</translation>
    </message>
    <message>
        <location filename="Status.cpp" line="73"/>
        <source>IsBusy2</source>
        <translation>IsBusy2</translation>
    </message>
    <message>
        <location filename="Status.cpp" line="74"/>
        <source>Busy2 Units</source>
        <translation>Busy2 Units</translation>
    </message>
    <message>
        <source>Clear Busys</source>
        <translation type="obsolete">Clear Busys</translation>
    </message>
    <message>
        <location filename="Status.cpp" line="129"/>
        <location filename="Status.cpp" line="162"/>
        <source>Clear</source>
        <translation>クリア</translation>
    </message>
    <message>
        <location filename="Status.cpp" line="183"/>
        <source>Encorder</source>
        <translation>エンコーダ</translation>
    </message>
    <message>
        <location filename="Status.cpp" line="189"/>
        <source>   </source>
        <translation></translation>
    </message>
    <message>
        <location filename="Status.cpp" line="193"/>
        <source>Set</source>
        <translation>設定</translation>
    </message>
    <message>
        <location filename="Status.cpp" line="222"/>
        <source>Connected</source>
        <translation>接続</translation>
    </message>
    <message>
        <location filename="Status.cpp" line="225"/>
        <source>not Connected</source>
        <translation>未接続</translation>
    </message>
</context>
<context>
    <name>TYView</name>
    <message>
        <location filename="TYView.cpp" line="10"/>
        <source>By clicking the &apos;A. Scale&apos; (Auto Scale) button at left-bottom corner, the auto-scale mode is toggled on and off.</source>
        <translation>左下の「A. Scale」 (Auto Scale)と書かれたボタンを押すことで、オートスケールモードをオン・オフできます。</translation>
    </message>
    <message>
        <location filename="TYView.cpp" line="175"/>
        <source>%1</source>
        <translation>%1</translation>
    </message>
</context>
<context>
    <name>XView</name>
    <message>
        <location filename="XView.ui" line="17"/>
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
        <location filename="XYView.cpp" line="10"/>
        <source>By clicking the &apos;A. Scale&apos; (Auto Scale) button at left-bottom corner, the auto-scale mode is toggled on and off.</source>
        <translation>左下の「A. Scale」 (Auto Scale)と書かれたボタンを押すことで、オートスケールモードをオン・オフできます。</translation>
    </message>
</context>
</TS>
