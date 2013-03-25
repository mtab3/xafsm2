<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="ja_JP">
<context>
    <name>AUnit</name>
    <message>
        <location filename="AUnit.cpp" line="262"/>
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
        <location filename="Conditions.ui" line="81"/>
        <source>Encorder is always used for Recorded Th</source>
        <translation>θとして常にエンコーダの値を記録する</translation>
    </message>
    <message>
        <location filename="Conditions.ui" line="91"/>
        <source>Selected Encorder or &apos;calc from PM&apos; is used</source>
        <translation>エンコーダかパルスモータの換算値かを選択する</translation>
    </message>
    <message>
        <source>No normalization</source>
        <translation type="obsolete">計測時間の補正無し</translation>
    </message>
    <message>
        <location filename="Conditions.ui" line="122"/>
        <source>No normalization    (now &apos;No&apos; shold be selected always)</source>
        <translation>規格化なし(今は、つねにこれを選んで下さい)</translation>
    </message>
    <message>
        <location filename="Conditions.ui" line="132"/>
        <source>Normalize the SSD-measured value by RealTime</source>
        <translation>SSDの測定値を「RealTime」で規格化する</translation>
    </message>
    <message>
        <location filename="Conditions.ui" line="139"/>
        <source>Normalize the SSD-measured value by LiveTime</source>
        <translation>SSDの測定値を「LiveTime」で規格化する</translation>
    </message>
    <message>
        <location filename="Conditions.ui" line="167"/>
        <source>Some additional information is recorded 
with usual 9809 format datas.</source>
        <translation>9809形式の通常のデータに加えて、追加の情報を行末に記録する(エンコーダ読み、パルスモータ換算両方の角度とエネルギー)。</translation>
    </message>
    <message>
        <location filename="Conditions.ui" line="171"/>
        <source>Record Addicional Informations for XAFS measurement</source>
        <translation>XAFS測定の記録ファイルに付加情報を含める</translation>
    </message>
    <message>
        <source>Use 1303 format to record XAFS measurement</source>
        <translation type="obsolete">XAFS測定の記録ファイルに 1303 フォーマットを使用する</translation>
    </message>
    <message>
        <location filename="Conditions.ui" line="50"/>
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
        <location filename="MainWindow.ui" line="64"/>
        <source>View 1</source>
        <translation>View1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="70"/>
        <source>View 2</source>
        <translation>View2</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="76"/>
        <source>View 3</source>
        <translation>View3</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="82"/>
        <source>View 4</source>
        <translation>View4</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="88"/>
        <source>View5</source>
        <translation>View5</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="94"/>
        <source>View6</source>
        <translation>View6</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="100"/>
        <source>View7</source>
        <translation>View7</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="106"/>
        <source>View8</source>
        <translation>View8</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="112"/>
        <source>View9</source>
        <translation>View9</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="118"/>
        <source>View10</source>
        <translation>View10</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="339"/>
        <source>Select an Atom</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="342"/>
        <source>Select a Target Atom</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="361"/>
        <source>Periodic Table</source>
        <translation>周期表</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="374"/>
        <source>Target</source>
        <translation>対象元素</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="387"/>
        <source>Edge</source>
        <translation>対象殻</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="403"/>
        <source>Select an absorption edge</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="415"/>
        <source>  Edge</source>
        <translation>エッジ位置</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="220"/>
        <source>KeV  </source>
        <translation>keV</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="467"/>
        <source>Deg.</source>
        <translation>度</translation>
    </message>
    <message>
        <source>KeV</source>
        <translation type="obsolete">keV</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="213"/>
        <source>Deg.  </source>
        <translation>度</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="206"/>
        <location filename="MainWindow.ui" line="236"/>
        <source>10.0000</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="42"/>
        <source>.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="150"/>
        <source>Cur. Pos.: </source>
        <translation>現在位置</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="172"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Usually the &amp;quot;Encorder&amp;quot; should be selected.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;通常は &amp;quot;エンコーダ&amp;quot;を選択&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="175"/>
        <source>Encorder</source>
        <translation>エンコーダ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="185"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;The &amp;quot;Calc from Pulse&amp;quot; should not be used for usual measurement.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;パルスモータのパルスから計算した角度。通常は&amp;quot;エンコーダ&amp;quot;を選択&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="188"/>
        <source>Calc from Pulse</source>
        <translation>パルス換算</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="292"/>
        <source>mA</source>
        <translation>mA</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="299"/>
        <source>RIng Current</source>
        <translation>リング電流</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="487"/>
        <source>Hide Tabs of Measurements and Settings</source>
        <translation>測定・條件設定のタブ部分を隠します</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="490"/>
        <source>Hide</source>
        <translation>隠</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="530"/>
        <source>Measurement</source>
        <translation>XAFS測定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="646"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Select an ion-chamber for I0.   The recommended gas is displayed in the following field.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>I0 の計測に使うイオンチャンバを選ぶと、推奨される混合ガスが次の欄に表示されます。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="653"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Recommended gas for I0 chamber.&lt;/p&gt;&lt;p&gt;To use the recommended gas, you need to exchange it manually.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>I0 チャンバに使用するお勧め混合ガス。
表示されたガスに自動的には切り替わらないので手動で切り替える必要があります。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="660"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Select an ion-chamber for I1.   The recommended gas is displayed in the following field.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>I1 の計測に使うイオンチャンバを選ぶと、推奨される混合ガスが次の欄に表示されます。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="667"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Recommended gas for I1 chamber.&lt;/p&gt;&lt;p&gt;To use the recommended gas, you need to exchange it manually.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>I1 チャンバに使用するお勧め混合ガス。
表示されたガスに自動的には切り替わらないので手動で切り替える必要があります。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="768"/>
        <source>Range of the device selected to measure A1.</source>
        <translation>「その他1」を測定する検出器のレンジ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="829"/>
        <source>Range of the device selected to measure I0.</source>
        <translation>I0 を測定する検出器のレンジ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="845"/>
        <source>Range of the device selected to measure I1.</source>
        <translation>I1 を測定する検出器のレンジ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="874"/>
        <source>Range of the device selected to measure A2.</source>
        <translation>「その他2」を測定する検出器のレンジ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="910"/>
        <source>Use novel 1303 data format to record the measured data instead of the usual 9809 format.</source>
        <translation>従来の 9809 フォーマットの代わりに 1303 フォーマットを用いて測定結果を記録する。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="913"/>
        <source>Use 1303 Format</source>
        <translation>1303フォーマット使用</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1026"/>
        <source>When 19ch SSD is used for the measurement, the MCA spectra for each channel is recorded at every measured points.</source>
        <translation>19ch SSDを使用する際、測定の各ステップでの MCA スペクトルを全て(全チャンネル、全ステップ)記録する。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1029"/>
        <source>Record MCA Spectra</source>
        <translation>MCAスペクトル記録</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1598"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;To measure near edge region precisely.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;XANES: ニアエッジ領域を詳細に測定&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1604"/>
        <source>Std. XANES</source>
        <translation>標準XANES</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1611"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Both EXAFS and XAFNES regions are measured precisely at once.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;XAFS: XANES領域とEXAFS領域の両方を詳細に測定&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1617"/>
        <source>Std. XAFS</source>
        <translation>標準XAFS</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1642"/>
        <location filename="MainWindow.ui" line="2556"/>
        <location filename="MainWindow.ui" line="3495"/>
        <location filename="MainWindow.ui" line="5008"/>
        <source>Save</source>
        <translation>保存</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1667"/>
        <source>Load</source>
        <translation>読込</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1680"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;To measure focusing on the &apos;EXAFS&apos; region. &lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;EXAFS: EXAFS領域に重点を置いた測定&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1686"/>
        <source>Std. EXAFS</source>
        <translation>標準EXAFS</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1737"/>
        <location filename="MainWindow.ui" line="4034"/>
        <location filename="MainWindow.ui" line="5354"/>
        <source>1</source>
        <translation>1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1757"/>
        <location filename="MainWindow.ui" line="4370"/>
        <location filename="MainWindow.ui" line="5403"/>
        <source>2</source>
        <translation>2</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1781"/>
        <location filename="MainWindow.ui" line="4053"/>
        <location filename="MainWindow.ui" line="5117"/>
        <source>3</source>
        <translation>3</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1949"/>
        <location filename="MainWindow.ui" line="4072"/>
        <location filename="MainWindow.ui" line="5167"/>
        <source>4</source>
        <translation>4</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1973"/>
        <location filename="MainWindow.ui" line="4091"/>
        <location filename="MainWindow.ui" line="5180"/>
        <source>5</source>
        <translation>5</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2011"/>
        <location filename="MainWindow.ui" line="4110"/>
        <location filename="MainWindow.ui" line="5193"/>
        <source>6</source>
        <translation>6</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2144"/>
        <location filename="MainWindow.ui" line="4129"/>
        <location filename="MainWindow.ui" line="5206"/>
        <source>7</source>
        <translation>7</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2050"/>
        <source>StartP</source>
        <translation>始点</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2069"/>
        <source>Step</source>
        <translation>間隔</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2082"/>
        <source>Steps</source>
        <translation>刻数</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1828"/>
        <source>Blocks: </source>
        <translation>ブロック数</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1838"/>
        <source>Dwell [s]</source>
        <translation>計測時間[秒]</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1887"/>
        <source>Set all dwell times at once</source>
        <translation>全計測時間を一斉に設定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1897"/>
        <source>All: </source>
        <translation>All: </translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1255"/>
        <source>Data File Name</source>
        <translation>データファイル</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1284"/>
        <source>...</source>
        <translation>選択</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1295"/>
        <source>Comment</source>
        <translation>コメント</translation>
    </message>
    <message>
        <source>Detecotr Select</source>
        <translation type="obsolete">検出器選択</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="599"/>
        <source>    I0</source>
        <translation>　I0</translation>
    </message>
    <message>
        <source>I</source>
        <translation type="obsolete">I</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="717"/>
        <source>Aux. 1</source>
        <translation>その他 1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="733"/>
        <source>Aux. 2</source>
        <translation>その他 2</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="589"/>
        <source>19ch SSD</source>
        <translation>19ch SSD</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="950"/>
        <source>The change of the repitation No. is available, even after starting measurement.
Though, other parameters are fixed at the value on starting.</source>
        <translation>「繰返し回数」は測定開始後も変更可能(その他の測定に関わるパラメータは測定後に変更しても反映されない)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="967"/>
        <source>Repeat</source>
        <translation>繰返し</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="984"/>
        <source>On Finish</source>
        <translation>終了時動作</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1003"/>
        <location filename="MainWindow.ui" line="4594"/>
        <location filename="Measurement.cpp" line="116"/>
        <location filename="MwMeas.cpp" line="850"/>
        <location filename="MwSSDSetUp.cpp" line="465"/>
        <source>Start</source>
        <translation>開始</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1019"/>
        <location filename="MwMeas.cpp" line="854"/>
        <location filename="MwMeas.cpp" line="877"/>
        <location filename="MwMeas.cpp" line="912"/>
        <source>Pause</source>
        <translation>一時停止</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1497"/>
        <source>Check Cond.</source>
        <translation>条件確認</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1504"/>
        <source>Total points which will be measured</source>
        <translation>総測定点数</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1507"/>
        <source>Total points which will be measured
</source>
        <translation>総測定点数</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1520"/>
        <source>Points:</source>
        <translation>点数:</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1527"/>
        <source>Total measurement time</source>
        <translation>予想測定時間</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1539"/>
        <source>Time:</source>
        <translation>時間:</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1102"/>
        <source>Meas. Bacground Time.[s] </source>
        <translation>バックグラウンド計測時間 [s] </translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1203"/>
        <source>Meas. Background</source>
        <translation>バックグラウンド計測</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1167"/>
        <source>Meas. before Scan</source>
        <translation>測定前にバックグラウンド計測</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1137"/>
        <source>Show BGs</source>
        <oldsource>Show Measured</oldsource>
        <translation>計測値表示</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1636"/>
        <source>Save the block definitions.</source>
        <translation>測定ブロック設定のセーブ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1661"/>
        <source>Load the saved block definitions.</source>
        <translation>測定ブロック設定の読込</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1370"/>
        <source>Start Time</source>
        <translation>開始時間</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1399"/>
        <source>Now</source>
        <translation>現在時間</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1412"/>
        <source>End TIme</source>
        <translation>終了時間</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4390"/>
        <location filename="MainWindow.ui" line="5459"/>
        <source>Down</source>
        <translation>下流</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4410"/>
        <location filename="MainWindow.ui" line="5479"/>
        <source>Stream</source>
        <translation> </translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4430"/>
        <location filename="MainWindow.ui" line="5499"/>
        <source>Up</source>
        <translation>上流</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4507"/>
        <source>Show Energy</source>
        <translation>エネルギー表示</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4517"/>
        <source>Fit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5880"/>
        <source>21</source>
        <translation>21</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5899"/>
        <source>23</source>
        <translation>23</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5918"/>
        <source>02</source>
        <translation>02</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5937"/>
        <source>22</source>
        <translation>22</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5956"/>
        <source>19</source>
        <translation>19</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6089"/>
        <source>03</source>
        <translation>03</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6165"/>
        <source>09</source>
        <translation>09</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6184"/>
        <source>08</source>
        <translation>08</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6203"/>
        <source>07</source>
        <translation>07</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6222"/>
        <source>06</source>
        <translation>06</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6241"/>
        <source>05</source>
        <translation>05</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6266"/>
        <source>24</source>
        <translation>24</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6288"/>
        <source>01</source>
        <translation>01</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6307"/>
        <source>04</source>
        <translation>04</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6329"/>
        <source>20</source>
        <translation>20</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2188"/>
        <source>Set up Conditions</source>
        <translation>条件設定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2841"/>
        <location filename="MainWindow.ui" line="2870"/>
        <location filename="MainWindow.ui" line="2913"/>
        <location filename="MainWindow.ui" line="2972"/>
        <location filename="MainWindow.ui" line="3290"/>
        <location filename="MwSetup.cpp" line="520"/>
        <source>Go</source>
        <translation>移動</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2877"/>
        <source>Move To (2)</source>
        <translation>移動先 (2)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2920"/>
        <source>Move To (3)</source>
        <translation>移動先 (3)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2979"/>
        <source>Move To (4)</source>
        <translation>移動先 (4)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3002"/>
        <source> Speed </source>
        <translation>スピード</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3064"/>
        <source>H</source>
        <translation>H</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3098"/>
        <source>M</source>
        <translation>M</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3135"/>
        <source>L</source>
        <translation>L</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3160"/>
        <source>Move Monochro</source>
        <translation>分光器回転</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3182"/>
        <source>Move To (1)</source>
        <translation>移動先 (1)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2467"/>
        <source>0.1</source>
        <translation>0.1</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2445"/>
        <source>Value</source>
        <translation>計測値</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2438"/>
        <source>Dwell T.</source>
        <translation>計測時間</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2537"/>
        <location filename="MainWindow.ui" line="3536"/>
        <location filename="MainWindow.ui" line="4994"/>
        <source>Select</source>
        <translation>選択</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2489"/>
        <location filename="MwSetup.cpp" line="749"/>
        <source>Mon. Start</source>
        <translation>モニタ開始</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2403"/>
        <location filename="MainWindow.ui" line="2580"/>
        <location filename="MainWindow.ui" line="2600"/>
        <source>0.000</source>
        <translation>0.000</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2425"/>
        <source>Monitor Sensors</source>
        <translation>検出器モニタ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3616"/>
        <source> Dwell [s]:</source>
        <translation>計測時間[s]:</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3672"/>
        <location filename="Scan.cpp" line="76"/>
        <source>Scan</source>
        <translation>スキャン開始</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3702"/>
        <source>StartP:</source>
        <translation>始点:</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3456"/>
        <location filename="MainWindow.ui" line="3609"/>
        <source>Abs</source>
        <translation>絶対</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3476"/>
        <source>EndP:</source>
        <translation>終点:</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3712"/>
        <source>Step:</source>
        <translation>間隔:</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3565"/>
        <location filename="MainWindow.ui" line="3587"/>
        <source>Rel</source>
        <translation>相対</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3297"/>
        <location filename="MainWindow.ui" line="4954"/>
        <source>Save:</source>
        <translation>保存ファイル:</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3307"/>
        <source>puls</source>
        <translation>puls</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3339"/>
        <source>Move To</source>
        <translation>移動先</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3372"/>
        <source>unit</source>
        <translation>単位</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3401"/>
        <source>Position</source>
        <translation>現在位置</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3418"/>
        <source>Monitor:</source>
        <translation>検出器:</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3796"/>
        <location filename="MainWindow.ui" line="3827"/>
        <location filename="MainWindow.ui" line="3994"/>
        <location filename="MainWindow.ui" line="5142"/>
        <source>0</source>
        <translation>0</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3747"/>
        <source>Scan/Move a Motor</source>
        <translation>移動/スキャン</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="441"/>
        <source>KeV   </source>
        <translation>keV</translation>
    </message>
    <message>
        <source>Measure before Scan</source>
        <translation type="obsolete">スキャン前に計測</translation>
    </message>
    <message>
        <location filename="Dark.cpp" line="161"/>
        <location filename="MwMeas.cpp" line="838"/>
        <source>Measure Background</source>
        <oldsource>Back Ground</oldsource>
        <translation>バックグラウンド計測</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1183"/>
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
        <location filename="MainWindow.ui" line="1196"/>
        <source>When it is displayed as &quot;Shutter Close!&quot;, check the shutter is closed and push the button.
When it is displayed as &quot;Shutter Open!, check the shutter is opend and push the button.</source>
        <translation>「シャッター CLOSE 確認」と表示されたときは、シャッターが閉まっていることを確認してボタンを押して下さい。
「シャッター OPEN 確認」と表示されたときは、シャッターが開いていることを確認してボタンを押して下さい。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="1074"/>
        <source>Back ground</source>
        <translation>バックグラウンド</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3848"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;When the &apos;Normalize&apos; is checked, the value of the detector selected as &apos;Monitor&apos; is normalized by the value of the detecter selected in the following selection-box.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;「規格化」がチェックされていると、「計測器:」で選択した計測器の値を「規格化」で選択した計測器の値で規格化します。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3851"/>
        <source>Normalize</source>
        <translation>規格化</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3864"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;The value of the selected detecter is always (with or without check the &apos;Normalize&apos;) recorded with the value of the detecter selected as &apos;Monitor&apos; for the scan.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>「規格化」で選択した測定器の値は、「規格化」をチェックしていてもしていなくても、主たる計測対象の「計測器：」で選択した機器の測定値と共に常に測定ファイルに記録されます。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2200"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Select measurement renge of the sensor.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;測定器の測定レンジを選択して下さい。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2236"/>
        <source>Select Range</source>
        <translation>レンジ選択</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2285"/>
        <location filename="MainWindow.ui" line="2683"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Get the selected range of the selected sensor.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>指定した計測器の現在のレンジ取得</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2291"/>
        <source>Get Range</source>
        <translation>レンジ取得</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2298"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Get the selected ranges of all of the sensors.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;レンジ選択のある全計測器の現在のレンジを取得する。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2304"/>
        <source>Get All Range</source>
        <translation>全レンジ取得</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3893"/>
        <source>Set up SSD</source>
        <translation>SSD設定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4775"/>
        <source>At Cursor</source>
        <translation>カーソル点</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4532"/>
        <source>Log  </source>
        <translation>Log </translation>
    </message>
    <message>
        <source>eV  Gain</source>
        <translation type="obsolete">eV  ゲイン</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4625"/>
        <source>Clear</source>
        <translation>クリア</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4665"/>
        <source>Live</source>
        <translation>ライブ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4758"/>
        <source>Real</source>
        <translation>リアル</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4768"/>
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
        <location filename="MainWindow.ui" line="4836"/>
        <source>u-sec</source>
        <translation>μs</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4843"/>
        <source>eV</source>
        <translation>eV</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4856"/>
        <source>mV/keV</source>
        <translation>mV/keV</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4869"/>
        <source>ROI  Start</source>
        <translation>積分始点</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4882"/>
        <source>Peak. T.</source>
        <translation>ピーキング時間</translation>
    </message>
    <message>
        <source>eV  Carib.</source>
        <translation type="obsolete">eV  較正</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4903"/>
        <source>sec</source>
        <translation>秒</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4910"/>
        <source>Thresh.</source>
        <translation>閾値</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4930"/>
        <source>Dyn. Range</source>
        <translation>ダイナミックレンジ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4943"/>
        <source>SSD No.</source>
        <translation>SSD チャンネル</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2154"/>
        <location filename="MainWindow.ui" line="4148"/>
        <location filename="MainWindow.ui" line="5219"/>
        <source>8</source>
        <translation>8</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2021"/>
        <location filename="MainWindow.ui" line="4170"/>
        <location filename="MainWindow.ui" line="5235"/>
        <source>9</source>
        <translation>9</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2173"/>
        <source>SetUp BLKs</source>
        <translation>測定ブロック設定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="626"/>
        <source>Select Detectors</source>
        <translation>検出器選択</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="943"/>
        <source>Start Meas.</source>
        <translation>測定開始</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5055"/>
        <source>Set Up a SSD Channel</source>
        <translation>SSDの各チャンネルの設定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4189"/>
        <location filename="MainWindow.ui" line="5248"/>
        <location filename="MainWindow.ui" line="6146"/>
        <source>10</source>
        <translation>10</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4208"/>
        <location filename="MainWindow.ui" line="5261"/>
        <location filename="MainWindow.ui" line="6127"/>
        <source>11</source>
        <translation>11</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4227"/>
        <location filename="MainWindow.ui" line="5274"/>
        <location filename="MainWindow.ui" line="6108"/>
        <source>12</source>
        <translation>12</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4246"/>
        <location filename="MainWindow.ui" line="5287"/>
        <location filename="MainWindow.ui" line="6070"/>
        <source>13</source>
        <translation>13</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4265"/>
        <location filename="MainWindow.ui" line="5300"/>
        <location filename="MainWindow.ui" line="6013"/>
        <source>14</source>
        <translation>14</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4284"/>
        <location filename="MainWindow.ui" line="5313"/>
        <location filename="MainWindow.ui" line="5994"/>
        <source>15</source>
        <translation>15</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4303"/>
        <location filename="MainWindow.ui" line="5326"/>
        <location filename="MainWindow.ui" line="6051"/>
        <source>16</source>
        <translation>16</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4322"/>
        <location filename="MainWindow.ui" line="5367"/>
        <location filename="MainWindow.ui" line="6032"/>
        <source>17</source>
        <translation>17</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4341"/>
        <location filename="MainWindow.ui" line="5380"/>
        <location filename="MainWindow.ui" line="5975"/>
        <source>18</source>
        <translation>18</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3955"/>
        <source>Select a SSD ch (MCA spectrum)</source>
        <oldsource>Select a SSD ch</oldsource>
        <translation>SSD選択(MCAスペクトル)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5432"/>
        <source>Select SSD channels (XAFS meas.)</source>
        <oldsource>Select SSD channels</oldsource>
        <translation>複数SSD選択(積算/本測定対象)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="701"/>
        <source>I                 </source>
        <translation></translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2689"/>
        <source>Set Offset</source>
        <translation>設定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2711"/>
        <source>Check/Set Offset(Dark)</source>
        <translation>バックグラウンド確認／設定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2314"/>
        <source>Auto Rang</source>
        <translation>オートレンジ</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="3206"/>
        <source>Move a motor or scan by moving a motor while monitoring a sensor.</source>
        <translation>駆動軸を動かす、もしくは軸を動かしながらスキャンを行う。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2798"/>
        <source>Move the monochromator where you want.</source>
        <translation>分光器の角度設定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2940"/>
        <source>Select an unit for all.</source>
        <translation>全部の単位を一度に設定します。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2311"/>
        <source>When the sensor can be used in auto-range mode, the check button is active.</source>
        <translation>選択した検出器でオートレンジモードが使える場合、このチェックボタンが有効になります。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2505"/>
        <source>Record the data during the monitor.</source>
        <translation>モニータ時のデータを記録します。(モニタ開始前にチェック)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2508"/>
        <source>Record</source>
        <translation>記録</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="2550"/>
        <source>Save the monitored data after the monitoring.</source>
        <translation>(モニタ後)モニタしたデータを記録します。</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4474"/>
        <source>Disp. Elm. Names</source>
        <translation>元素名表示</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4490"/>
        <source>Select Elms.</source>
        <translation>表示元素選択</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4497"/>
        <source>Show Always</source>
        <translation>選択した元素を常に表示</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4545"/>
        <source>eV        Gain</source>
        <translation>eV       ゲイン</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4810"/>
        <source>      ROI end</source>
        <oldsource>       ROI end</oldsource>
        <translation>     積分終点</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4823"/>
        <source>         In ROI</source>
        <oldsource>          In ROI</oldsource>
        <translation>       積分値</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="4889"/>
        <source>eV       Carib.</source>
        <translation>eV        較正.</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5065"/>
        <source>Set All</source>
        <translation>Set All</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5558"/>
        <source>Read Data</source>
        <translation>データ読込</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5731"/>
        <source>Close View</source>
        <translation>View を閉じる</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5761"/>
        <source>Log / Record</source>
        <translation>ログ/記録</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5775"/>
        <source>Log File Name:</source>
        <translation>ログファイル名</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5798"/>
        <source>New</source>
        <translation>選択</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5824"/>
        <source>Add Log Comment:</source>
        <translation>ログコメント追加</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="5742"/>
        <source>Stat/Config</source>
        <translation>状態/設定</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6355"/>
        <source>&amp;File</source>
        <translation>ファイル(&amp;F)</translation>
    </message>
    <message>
        <source>&amp;Config</source>
        <translation type="obsolete">設定(&amp;C)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6364"/>
        <source>&amp;Quit</source>
        <translation>終了(&amp;Q)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6369"/>
        <source>Japanese</source>
        <translation>日本語</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6374"/>
        <source>English</source>
        <translation>English</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6379"/>
        <source>Set &amp;Lattice Const. Monochro Crystal</source>
        <translation>結晶・格子定数設定(&amp;L)</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="6384"/>
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
        <location filename="MainWindow.cpp" line="85"/>
        <source>Mono: %1 (%2 A)</source>
        <oldsource>Mono: %1 (%2 A)
</oldsource>
        <translation>分光結晶: %1 (%2 A)</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="243"/>
        <source>Encorder is set from %1 to %2</source>
        <translation>エンコーダの値を %1 から %2 にセットしました</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="305"/>
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
        <location filename="MainWindow.cpp" line="360"/>
        <source>No Scree is available!</source>
        <translation>使用可能な描画領域(View)がありません</translation>
    </message>
    <message>
        <location filename="Measurement.cpp" line="38"/>
        <source>Start Measurement!</source>
        <translation>測定開始</translation>
    </message>
    <message>
        <location filename="Measurement.cpp" line="50"/>
        <source>Writing Header.</source>
        <translation>ヘッダ記録</translation>
    </message>
    <message>
        <location filename="Measurement.cpp" line="105"/>
        <source>Meas: Repeat %1</source>
        <oldsource>Meas: Repeat %1
</oldsource>
        <translation>測定： %1回目</translation>
    </message>
    <message>
        <location filename="Measurement.cpp" line="111"/>
        <source>The Measurement has Finished</source>
        <translation>測定終了</translation>
    </message>
    <message>
        <location filename="Measurement.cpp" line="112"/>
        <source>Meas: Finished</source>
        <oldsource>Meas: Finished
</oldsource>
        <translation>測定: 終了</translation>
    </message>
    <message>
        <location filename="MwCommon.cpp" line="146"/>
        <location filename="MwCommon.cpp" line="158"/>
        <source>Absorption edge of the atom is out of range.</source>
        <translation>対象原子の吸収端が測定可能範囲外です。</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="46"/>
        <source>Return</source>
        <translation>原位置復帰</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="47"/>
        <source>Stay</source>
        <translation>その場に停止</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="59"/>
        <location filename="MwMeas.cpp" line="66"/>
        <location filename="MwMeas.cpp" line="72"/>
        <source>Cancel</source>
        <translation>キャンセル</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="60"/>
        <location filename="MwMeas.cpp" line="67"/>
        <location filename="MwMeas.cpp" line="73"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="61"/>
        <source>&lt;h1&gt;&lt;center&gt;Stop ?&lt;/center&gt;&lt;/h1&gt;</source>
        <translation>終了?</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="62"/>
        <source>Stop or Continue</source>
        <translation>停止／再開</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="68"/>
        <source>Over Write ?</source>
        <translation>上書きしますか?</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="74"/>
        <source>Have you seleced ?</source>
        <translation>選択済み？</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="400"/>
        <source>Points: </source>
        <translation>点数: </translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="410"/>
        <source>Time: </source>
        <translation>時間: </translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="552"/>
        <source>I0</source>
        <translation>I0</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="553"/>
        <source>mu(E)</source>
        <translation>μ(E)</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="554"/>
        <source>Energy</source>
        <translation></translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="555"/>
        <source>[keV]</source>
        <translation>[keV]</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="627"/>
        <source>Monochro is moving!</source>
        <translation>分光器回転中</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="631"/>
        <location filename="MwMeas.cpp" line="707"/>
        <source>Meas cannot Start : (%1) is disabled</source>
        <translation>(%1) が使用できないため測定を開始できません。</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="673"/>
        <source>Meas cannot Start : in MCA measurement</source>
        <translation>MCA 測定中のため XAFS 測定を開始できません</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="779"/>
        <source>Meas: Start %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</source>
        <translation>測定開始 %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="819"/>
        <source>Meas: Break %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</source>
        <translation>測定中断 %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="842"/>
        <source>Meas: Stopped %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</source>
        <translation>測定中断 : %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="864"/>
        <source>Meas: Pausing %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</source>
        <translation>測定中断中 %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="871"/>
        <location filename="MwMeas.cpp" line="906"/>
        <source>Meas: Resume %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</source>
        <translation>測定再開 %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="897"/>
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
        <location filename="MwSetup.cpp" line="546"/>
        <location filename="MwSetup.cpp" line="554"/>
        <location filename="MwSetup.cpp" line="634"/>
        <location filename="MwSetup.cpp" line="678"/>
        <location filename="MwSetup.cpp" line="689"/>
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
        <location filename="MwMeas.cpp" line="636"/>
        <source>Invalid block data.</source>
        <translation>測定ブロックの指定が誤っています。</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="640"/>
        <source>Data File is not Selected!</source>
        <translation>データファイルが選択されていません。</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="644"/>
        <source>Detectors are not selected properly!</source>
        <translation>測定のための検出器の指定が正しくありません。</translation>
    </message>
    <message>
        <location filename="Dark.cpp" line="69"/>
        <location filename="MwMeas.cpp" line="727"/>
        <source>Selected sensors [%1] and [%2] are conflicting.</source>
        <translation>選択した[%1]と[%2]は同時に使えません。</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="740"/>
        <source>The Sensor(s)%1 should be range selected.
Have you selected the range in &apos;Setup Condition&apos;</source>
        <translation>選択した計測器 %1 について、「設定」の画面でレンジを設定しましたか？</translation>
    </message>
    <message>
        <location filename="MwMeas.cpp" line="771"/>
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
        <location filename="MwMeas.cpp" line="786"/>
        <location filename="MwSSDSetUp.cpp" line="422"/>
        <location filename="MwSetup.cpp" line="467"/>
        <location filename="MwSetup.cpp" line="586"/>
        <location filename="MwSetup.cpp" line="727"/>
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
        <location filename="MwMeas.cpp" line="825"/>
        <location filename="MwMeas.cpp" line="903"/>
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
        <location filename="MwMeas.cpp" line="847"/>
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
        <location filename="MwSSDSetUp.cpp" line="281"/>
        <location filename="MwSSDSetUp.cpp" line="300"/>
        <source>Active</source>
        <translation>アクティブ</translation>
    </message>
    <message>
        <location filename="MwSSDSetUp.cpp" line="190"/>
        <source>MCA data is not valid</source>
        <translation>正しいMCAデータがありません。</translation>
    </message>
    <message>
        <location filename="MwSSDSetUp.cpp" line="194"/>
        <location filename="MwSetup.cpp" line="226"/>
        <source>Save file name is not selected</source>
        <translation>ファイル名が選択されていません。</translation>
    </message>
    <message>
        <location filename="MwSSDSetUp.cpp" line="205"/>
        <location filename="MwSetup.cpp" line="232"/>
        <location filename="MwSetup.cpp" line="653"/>
        <source>The file [%1] can not open to record the data</source>
        <translation>記録のためのファイル [%1]が開けません。</translation>
    </message>
    <message>
        <location filename="MwSSDSetUp.cpp" line="286"/>
        <location filename="MwSSDSetUp.cpp" line="306"/>
        <source>Inactive</source>
        <translation>インアクティブ</translation>
    </message>
    <message>
        <location filename="MwSSDSetUp.cpp" line="416"/>
        <source>Scan cannot Start : SSD is disabled</source>
        <translation>SSD が disable なので測定が開始できません。</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="222"/>
        <source>Scan data is not valid</source>
        <translation>正しいスキャンデータがありません。</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="365"/>
        <source>Current Position of [%1] : [%2] %3</source>
        <oldsource>Current Position of [%1] : [%2] %3
</oldsource>
        <translation>[%1]の現在位置 : [%2] %3</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="478"/>
        <location filename="MwSetup.cpp" line="482"/>
        <source>Setup: %1 : GoTo %2 : Speed %3</source>
        <oldsource>Setup: %1 : GoTo %2 : Speed %3
</oldsource>
        <translation>設定: %1 をスピード %3 で %2 に移動</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="505"/>
        <location filename="MwSetup.cpp" line="508"/>
        <source>Setup: %1 : Stopped at %2</source>
        <oldsource>Setup: %1 : Stopped at %2
</oldsource>
        <translation>設定: %1 を %2 で停止</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="530"/>
        <source>No drawing screen is available</source>
        <translation>使用可能な描画領域(View)がありません。</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="575"/>
        <source>Error: Scan Step is 0.</source>
        <translation>エラー: スキャンステップが 0 です。</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="580"/>
        <source>Scan Start (%1 %2)</source>
        <oldsource>Scan Start (%1 %2)
</oldsource>
        <translation>スキャン: 開始 (%1, %2)</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="641"/>
        <source>No drawing area is avairable</source>
        <translation>使用可能な描画領域(View)がありません。</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="647"/>
        <source>No Record file is selected</source>
        <translation>記録ファイルが選択されていません。</translation>
    </message>
    <message>
        <location filename="MwSetup.cpp" line="704"/>
        <source>	%1[%2]</source>
        <translation></translation>
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
        <location filename="ReadDef.cpp" line="103"/>
        <source>::Undefined Unit type [%1]</source>
        <translation>::Undefined Unit type [%1]</translation>
    </message>
    <message>
        <location filename="ReadDef.cpp" line="150"/>
        <source>Undefined Key word [%1]</source>
        <translation>Undefined Key word [%1]</translation>
    </message>
    <message>
        <location filename="ReadDef.cpp" line="231"/>
        <source>UIDs [%1](at line %2) and [%3](at line %4) are duplicated.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="ReadDef.cpp" line="234"/>
        <source>1st one is : Type[%1] Identifier[%2] Driver[%3] Node[%4]</source>
        <translation></translation>
    </message>
    <message>
        <location filename="ReadDef.cpp" line="236"/>
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
        <location filename="Stars.cpp" line="316"/>
        <source>Receive an event from Stars [%1]</source>
        <translation>Stars サーバからのイベント [%1]</translation>
    </message>
    <message>
        <location filename="Stars.cpp" line="333"/>
        <source>Receive an unricognized message from Stars [%1]</source>
        <translation>Stars サーバからの解釈できないメッセージ [%1]</translation>
    </message>
    <message>
        <location filename="Stars.cpp" line="335"/>
        <source>Others:  [%1] -&gt; [%2] : [%3] [%4]
</source>
        <translation>Others:  [%1] -&gt; [%2] : [%3] [%4</translation>
    </message>
    <message>
        <location filename="Stars.cpp" line="352"/>
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
