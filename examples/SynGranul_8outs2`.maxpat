{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 8,
			"minor" : 0,
			"revision" : 2,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"classnamespace" : "box",
		"rect" : [ 14.0, 59.0, 1097.0, 669.0 ],
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 1,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 1,
		"objectsnaponopen" : 1,
		"statusbarvisible" : 2,
		"toolbarvisible" : 0,
		"lefttoolbarpinned" : 0,
		"toptoolbarpinned" : 0,
		"righttoolbarpinned" : 0,
		"bottomtoolbarpinned" : 0,
		"toolbars_unpinned_last_save" : 0,
		"tallnewobj" : 0,
		"boxanimatetime" : 200,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"style" : "",
		"subpatcher_template" : "",
		"boxes" : [ 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-53",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 913.0, 447.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.451591000000001,
					"id" : "obj-49",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 913.0, 473.055542000000003, 68.0, 22.0 ],
					"text" : "nvoices $1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-47",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 973.0, 379.0, 48.0, 22.0 ],
					"text" : "/~ 360."
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-43",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 973.0, 350.0, 51.0, 22.0 ],
					"text" : "+~ 22.5"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-42",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 224.0, 426.0, 42.0, 22.0 ],
					"text" : "*~ 0.1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-40",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 224.0, 407.0, 37.0, 19.0 ],
					"text" : "noise~"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-52",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 994.0, 57.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-50",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 949.0, 55.0, 20.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-48",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 948.0, 152.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-46",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 948.0, 81.0, 58.0, 22.0 ],
					"text" : "metro 10"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-44",
					"maxclass" : "live.tab",
					"num_lines_patching" : 17,
					"num_lines_presentation" : 0,
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "", "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 871.0, 72.0, 32.0, 298.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_shortname" : "live.tab[1]",
							"parameter_enum" : [ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16" ],
							"parameter_type" : 2,
							"parameter_unitstyle" : 0,
							"parameter_longname" : "live.tab[1]"
						}

					}
,
					"varname" : "live.tab"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-41",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 871.0, 382.0, 77.0, 22.0 ],
					"text" : "envbuffer $1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-1",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 169.0, 422.0, 37.0, 19.0 ],
					"text" : "mtof~"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-2",
					"interpinlet" : 1,
					"maxclass" : "gain~",
					"multichannelvariant" : 0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 456.0, 483.0, 11.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-3",
					"maxclass" : "bargraf~",
					"numinlets" : 2,
					"numoutlets" : 3,
					"outlettype" : [ "float", "float", "float" ],
					"patching_rect" : [ 468.0, 483.0, 13.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-4",
					"interpinlet" : 1,
					"maxclass" : "gain~",
					"multichannelvariant" : 0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 429.0, 483.0, 11.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-5",
					"maxclass" : "bargraf~",
					"numinlets" : 2,
					"numoutlets" : 3,
					"outlettype" : [ "float", "float", "float" ],
					"patching_rect" : [ 441.0, 483.0, 13.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-6",
					"interpinlet" : 1,
					"maxclass" : "gain~",
					"multichannelvariant" : 0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 401.0, 483.0, 11.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-7",
					"maxclass" : "bargraf~",
					"numinlets" : 2,
					"numoutlets" : 3,
					"outlettype" : [ "float", "float", "float" ],
					"patching_rect" : [ 413.0, 483.0, 13.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-8",
					"interpinlet" : 1,
					"maxclass" : "gain~",
					"multichannelvariant" : 0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 373.0, 483.0, 11.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-9",
					"maxclass" : "bargraf~",
					"numinlets" : 2,
					"numoutlets" : 3,
					"outlettype" : [ "float", "float", "float" ],
					"patching_rect" : [ 385.0, 483.0, 13.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"hidden" : 1,
					"id" : "obj-10",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 432.0, 388.0, 40.0, 19.0 ],
					"text" : "dbtoa~"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"htricolor" : [ 0.87, 0.82, 0.24, 1.0 ],
					"id" : "obj-11",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 825.0, 508.0, 35.0, 19.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"tricolor" : [ 0.75, 0.75, 0.75, 1.0 ],
					"triscale" : 0.9
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-12",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 825.0, 486.0, 70.0, 19.0 ],
					"text" : "speedlim 200"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-13",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 825.0, 465.0, 66.0, 19.0 ],
					"text" : "route voices"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-14",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 507.0, 532.0, 39.0, 19.0 ],
					"text" : "update"
				}

			}
, 			{
				"box" : 				{
					"allowdrag" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-15",
					"items" : [ "(gmu.env.default.json)", ",", 1, "-", "tic-tic", ",", 2, "-", "expo", "sinsoidÂ?e", ",", 3, "-", "expo", "decay", ",", 4, "-", "expo", "sinux", 2, ",", 5, "-", "sweep", "mod", ",", 6, "-", "expo", "bump", ",", 7, "-", "deux", "freq", ",", 8, "-", "sweep", "gratt", ",", 9, "-", "bpf", "sin", 1, ",", 10, "-", "bpf", "trapez", ",", 11, "-", "bpf", "sin", 2, ",", 12, "-", "blobby", ",", 13, "-", "gratz", "blob", ",", 14, "-", "crickss", ",", 15, "-", "sin", "noise", ",", 16, "-", "sin", "noise", 3, ",", 17, "-", "sin", "noise", 2, ",", 18, "-", "gratz", "blob", "rehauss", ",", 19, "-", "gauss_bruitend", ",", 20, "-", "gauss_bruitend_rept", ",", 21, "-", "bpf", "trapez", "rept", ",", 22, "-", "cluster", "bruiteux", ",", 23, "-", "clust", "bruit", "deux", ",", 24, "-", "peigne-gauss", ",", 25, "-", "sin", "noise", 4 ],
					"maxclass" : "umenu",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 553.0, 514.0, 169.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-16",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 710.0, 567.0, 32.0, 19.0 ],
					"text" : "view"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-17",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 654.0, 594.0, 54.0, 19.0 ],
					"text" : "view_env"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 10.402851999999999,
					"id" : "obj-18",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 5,
					"outlettype" : [ "", "", "", "", "" ],
					"patching_rect" : [ 553.0, 564.069336000000021, 154.0, 20.0 ],
					"text" : "gmu.envelope.editor env1 4096",
					"varname" : "Envelope_editor"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-19",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 159.0, 466.0, 39.0, 19.0 ],
					"text" : "update"
				}

			}
, 			{
				"box" : 				{
					"active" : 					{
						"Envelope_editor::live.tab" : 0
					}
,
					"autorestore" : "syngran_sto.xml",
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-20",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 33.0, 573.0, 177.0, 19.0 ],
					"saved_object_attributes" : 					{
						"client_rect" : [ 0, 0, 640, 240 ],
						"parameter_enable" : 0,
						"parameter_mappable" : 0,
						"storage_rect" : [ 0, 0, 640, 240 ]
					}
,
					"text" : "pattrstorage syngran_sto @greedy 1",
					"varname" : "syngran_sto"
				}

			}
, 			{
				"box" : 				{
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-21",
					"lockeddragscroll" : 1,
					"maxclass" : "bpatcher",
					"name" : "storage_helper.maxpat",
					"numinlets" : 3,
					"numoutlets" : 2,
					"offset" : [ 0.0, 0.0 ],
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 33.0, 495.0, 172.0, 74.0 ],
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-22",
					"maxclass" : "newobj",
					"numinlets" : 8,
					"numoutlets" : 0,
					"patching_rect" : [ 265.0, 583.0, 202.0, 19.0 ],
					"text" : "dac~ 1 2 3 4 5 6 7 8"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-23",
					"interpinlet" : 1,
					"maxclass" : "gain~",
					"multichannelvariant" : 0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 344.0, 482.0, 11.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-24",
					"maxclass" : "bargraf~",
					"numinlets" : 2,
					"numoutlets" : 3,
					"outlettype" : [ "float", "float", "float" ],
					"patching_rect" : [ 356.0, 482.0, 13.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-25",
					"interpinlet" : 1,
					"maxclass" : "gain~",
					"multichannelvariant" : 0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 317.0, 482.0, 11.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-26",
					"maxclass" : "bargraf~",
					"numinlets" : 2,
					"numoutlets" : 3,
					"outlettype" : [ "float", "float", "float" ],
					"patching_rect" : [ 329.0, 482.0, 13.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-27",
					"interpinlet" : 1,
					"maxclass" : "gain~",
					"multichannelvariant" : 0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 289.0, 482.0, 11.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-28",
					"maxclass" : "bargraf~",
					"numinlets" : 2,
					"numoutlets" : 3,
					"outlettype" : [ "float", "float", "float" ],
					"patching_rect" : [ 301.0, 482.0, 13.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-29",
					"interpinlet" : 1,
					"maxclass" : "gain~",
					"multichannelvariant" : 0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 261.0, 482.0, 11.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-30",
					"maxclass" : "bargraf~",
					"numinlets" : 2,
					"numoutlets" : 3,
					"outlettype" : [ "float", "float", "float" ],
					"patching_rect" : [ 273.0, 482.0, 13.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-31",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 42.0, 424.0, 35.0, 19.0 ],
					"text" : "poll 1"
				}

			}
, 			{
				"box" : 				{
					"args" : [ "pan", 0.07, 0.0, 360.0, 20.0, 1, "0./1." ],
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-32",
					"lockeddragscroll" : 1,
					"maxclass" : "bpatcher",
					"name" : "tranche_ctrl5.maxpat",
					"numinlets" : 5,
					"numoutlets" : 2,
					"offset" : [ -967.0, 0.0 ],
					"outlettype" : [ "signal", "" ],
					"patching_rect" : [ 698.0, 30.0, 131.0, 388.0 ],
					"varname" : "pan_ctrl",
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"args" : [ "length", 100.0, 0.0, 1000.0, 20.0, 1, "ms" ],
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-33",
					"lockeddragscroll" : 1,
					"maxclass" : "bpatcher",
					"name" : "tranche_ctrl5.maxpat",
					"numinlets" : 5,
					"numoutlets" : 2,
					"offset" : [ -967.0, 0.0 ],
					"outlettype" : [ "signal", "" ],
					"patching_rect" : [ 565.0, 29.0, 131.0, 388.0 ],
					"varname" : "length_ctrl",
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"args" : [ "amp", -6.0, -45.0, 0.0, 20.0, 1, "dB" ],
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-34",
					"lockeddragscroll" : 1,
					"maxclass" : "bpatcher",
					"name" : "tranche_ctrl5.maxpat",
					"numinlets" : 5,
					"numoutlets" : 2,
					"offset" : [ -967.0, 0.0 ],
					"outlettype" : [ "signal", "" ],
					"patching_rect" : [ 432.0, 28.0, 131.0, 388.0 ],
					"varname" : "amp_ctrl",
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"args" : [ "phase", 0.0, 0.0, 1.0, 20.0, 1, "phi" ],
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-35",
					"lockeddragscroll" : 1,
					"maxclass" : "bpatcher",
					"name" : "tranche_ctrl5.maxpat",
					"numinlets" : 5,
					"numoutlets" : 2,
					"offset" : [ -967.0, 0.0 ],
					"outlettype" : [ "signal", "" ],
					"patching_rect" : [ 299.0, 29.0, 131.0, 388.0 ],
					"varname" : "phase_ctrl",
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"args" : [ "note", 0.0, 0.0, 127.0, 20.0, 1, "midi" ],
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-36",
					"lockeddragscroll" : 1,
					"maxclass" : "bpatcher",
					"name" : "tranche_ctrl5.maxpat",
					"numinlets" : 5,
					"numoutlets" : 2,
					"offset" : [ -967.0, 0.0 ],
					"outlettype" : [ "signal", "" ],
					"patching_rect" : [ 165.0, 26.0, 131.0, 388.0 ],
					"varname" : "note_ctrl",
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-37",
					"maxclass" : "ezdac~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 212.0, 532.0, 33.0, 33.0 ]
				}

			}
, 			{
				"box" : 				{
					"args" : [ "trig", 3.0, -36.0, 92.0, 20.0, 1 ],
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-38",
					"lockeddragscroll" : 1,
					"maxclass" : "bpatcher",
					"name" : "trig_ctrl5.maxpat",
					"numinlets" : 5,
					"numoutlets" : 2,
					"offset" : [ -985.0, 0.0 ],
					"outlettype" : [ "signal", "signal" ],
					"patching_rect" : [ 33.0, 24.0, 131.0, 388.0 ],
					"varname" : "trig_ctrl",
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-39",
					"maxclass" : "newobj",
					"numinlets" : 7,
					"numoutlets" : 9,
					"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "" ],
					"patching_rect" : [ 20.0, 446.0, 830.0, 19.0 ],
					"text" : "synGranul~ env123 8"
				}

			}
, 			{
				"box" : 				{
					"editor_rect" : [ 980.0, 298.0, 300.0, 300.0 ],
					"embed" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-45",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ],
					"patching_rect" : [ 948.0, 121.0, 103.0, 22.0 ],
					"saved_object_attributes" : 					{
						"embed" : 0,
						"name" : "env-choose",
						"parameter_enable" : 0,
						"parameter_mappable" : 0,
						"range" : 128,
						"showeditor" : 0,
						"size" : 16
					}
,
					"showeditor" : 1,
					"text" : "table env-choose"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-39", 1 ],
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-39", 3 ],
					"source" : [ "obj-10", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-11", 0 ],
					"source" : [ "obj-12", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-12", 0 ],
					"source" : [ "obj-13", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-18", 0 ],
					"midpoints" : [ 516.5, 561.0, 562.5, 561.0 ],
					"source" : [ "obj-14", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-18", 1 ],
					"source" : [ "obj-15", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 1 ],
					"source" : [ "obj-16", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-15", 0 ],
					"midpoints" : [ 663.75, 590.0, 734.0, 590.0, 734.0, 503.0, 562.5, 503.0 ],
					"source" : [ "obj-18", 3 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-18", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-39", 0 ],
					"hidden" : 1,
					"source" : [ "obj-18", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-21", 1 ],
					"midpoints" : [ 168.5, 488.0, 119.0, 488.0 ],
					"source" : [ "obj-19", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-22", 7 ],
					"order" : 1,
					"source" : [ "obj-2", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-3", 0 ],
					"hidden" : 1,
					"order" : 0,
					"source" : [ "obj-2", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-21", 0 ],
					"hidden" : 1,
					"source" : [ "obj-20", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-20", 0 ],
					"hidden" : 1,
					"source" : [ "obj-21", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-22", 3 ],
					"order" : 1,
					"source" : [ "obj-23", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-24", 0 ],
					"hidden" : 1,
					"order" : 0,
					"source" : [ "obj-23", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-8", 0 ],
					"hidden" : 1,
					"source" : [ "obj-23", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-22", 2 ],
					"order" : 1,
					"source" : [ "obj-25", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-26", 0 ],
					"hidden" : 1,
					"order" : 0,
					"source" : [ "obj-25", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-22", 1 ],
					"order" : 1,
					"source" : [ "obj-27", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-28", 0 ],
					"hidden" : 1,
					"order" : 0,
					"source" : [ "obj-27", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-22", 0 ],
					"order" : 1,
					"source" : [ "obj-29", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-23", 0 ],
					"hidden" : 1,
					"order" : 0,
					"source" : [ "obj-29", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-25", 0 ],
					"hidden" : 1,
					"order" : 1,
					"source" : [ "obj-29", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-27", 0 ],
					"hidden" : 1,
					"order" : 2,
					"source" : [ "obj-29", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-30", 0 ],
					"hidden" : 1,
					"order" : 0,
					"source" : [ "obj-29", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-39", 0 ],
					"source" : [ "obj-31", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-43", 0 ],
					"source" : [ "obj-32", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-39", 4 ],
					"source" : [ "obj-33", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-10", 0 ],
					"hidden" : 1,
					"source" : [ "obj-34", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-39", 2 ],
					"source" : [ "obj-35", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"source" : [ "obj-36", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-39", 0 ],
					"source" : [ "obj-38", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-13", 0 ],
					"source" : [ "obj-39", 8 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-2", 0 ],
					"source" : [ "obj-39", 7 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-23", 0 ],
					"source" : [ "obj-39", 3 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-25", 0 ],
					"source" : [ "obj-39", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-27", 0 ],
					"midpoints" : [ 130.875, 475.0, 292.5, 475.0 ],
					"source" : [ "obj-39", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-29", 0 ],
					"midpoints" : [ 29.5, 475.0, 264.5, 475.0 ],
					"source" : [ "obj-39", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-4", 0 ],
					"source" : [ "obj-39", 6 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-6", 0 ],
					"source" : [ "obj-39", 5 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-8", 0 ],
					"source" : [ "obj-39", 4 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-22", 6 ],
					"order" : 1,
					"source" : [ "obj-4", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-5", 0 ],
					"hidden" : 1,
					"order" : 0,
					"source" : [ "obj-4", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-42", 0 ],
					"source" : [ "obj-40", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-39", 0 ],
					"midpoints" : [ 880.5, 429.0, 207.0, 429.0, 207.0, 417.0, 156.0, 417.0, 156.0, 426.0, 87.0, 426.0, 87.0, 441.0, 29.5, 441.0 ],
					"source" : [ "obj-41", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"source" : [ "obj-42", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-47", 0 ],
					"source" : [ "obj-43", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-41", 0 ],
					"source" : [ "obj-44", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-48", 0 ],
					"source" : [ "obj-45", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-45", 0 ],
					"source" : [ "obj-46", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-39", 5 ],
					"source" : [ "obj-47", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-44", 0 ],
					"midpoints" : [ 957.5, 174.0, 915.0, 174.0, 915.0, 69.0, 880.5, 69.0 ],
					"source" : [ "obj-48", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-39", 0 ],
					"source" : [ "obj-49", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-46", 0 ],
					"source" : [ "obj-50", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-46", 1 ],
					"source" : [ "obj-52", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-49", 0 ],
					"source" : [ "obj-53", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-22", 5 ],
					"order" : 1,
					"source" : [ "obj-6", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-7", 0 ],
					"hidden" : 1,
					"order" : 0,
					"source" : [ "obj-6", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-2", 0 ],
					"hidden" : 1,
					"order" : 0,
					"source" : [ "obj-8", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-22", 4 ],
					"order" : 1,
					"source" : [ "obj-8", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-4", 0 ],
					"hidden" : 1,
					"order" : 1,
					"source" : [ "obj-8", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-6", 0 ],
					"hidden" : 1,
					"order" : 2,
					"source" : [ "obj-8", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-9", 0 ],
					"hidden" : 1,
					"order" : 0,
					"source" : [ "obj-8", 0 ]
				}

			}
 ],
		"parameters" : 		{
			"obj-18::obj-229" : [ "live.tab", "live.tab", 0 ],
			"obj-44" : [ "live.tab[1]", "live.tab[1]", 0 ],
			"parameterbanks" : 			{

			}

		}
,
		"dependency_cache" : [ 			{
				"name" : "trig_ctrl5.maxpat",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/GMU-1.7-max7-64bit/abs",
				"patcherrelativepath" : "../abs",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "mslide_XY2MS.maxpat",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/GMU-1.7-max7-64bit/abs",
				"patcherrelativepath" : "../abs",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "sw.maxpat",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/_abs",
				"patcherrelativepath" : "../../_abs",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "mysw.maxpat",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/GMU-1.7-max7-64bit/abs",
				"patcherrelativepath" : "../abs",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "mychange.maxpat",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/GMU-1.7-max7-64bit/abs",
				"patcherrelativepath" : "../abs",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "setname.js",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/GMU-1.7-max7-64bit/js",
				"patcherrelativepath" : "../js",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "tranche_ctrl5.maxpat",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/GMU-1.7-max7-64bit/abs",
				"patcherrelativepath" : "../abs",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "storage_helper.maxpat",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/GMU-1.7-max7-64bit/abs",
				"patcherrelativepath" : "../abs",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "dialog_preset.maxpat",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/HoloLive_0.5/hololive.editor_0.5ß",
				"patcherrelativepath" : "../../HoloLive_0.5/hololive.editor_0.5ß",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "syngran_sto.xml",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/GMU-1.7-max7-64bit/examples",
				"patcherrelativepath" : ".",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "gmu.envelope.editor.maxpat",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/GMU-1.7-max7-64bit/abs/Envelope_Edit",
				"patcherrelativepath" : "../abs/Envelope_Edit",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "ad.toggle2.png",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/GMU-1.7-max7-64bit/abs/Envelope_Edit",
				"patcherrelativepath" : "../abs/Envelope_Edit",
				"type" : "PNG",
				"implicit" : 1
			}
, 			{
				"name" : "ad.toggle.png",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/GMU-1.7-max7-64bit/abs/Envelope_Edit",
				"patcherrelativepath" : "../abs/Envelope_Edit",
				"type" : "PNG",
				"implicit" : 1
			}
, 			{
				"name" : "gmu.env.bank.json",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/GMU-1.7-max7-64bit/examples",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "gmu.poly.buffer.maxpat",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/GMU-1.7-max7-64bit/abs/Envelope_Edit",
				"patcherrelativepath" : "../abs/Envelope_Edit",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "gmu.env.default.json",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/GMU-1.7-max7-64bit/examples",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "view_env.maxpat",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/GMU-1.7-max7-64bit/abs/Envelope_Edit",
				"patcherrelativepath" : "../abs/Envelope_Edit",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "synGranul~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "idleubutton.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "rand_dist_list~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "bargraf~.mxo",
				"type" : "iLaX"
			}
 ],
		"autosave" : 0
	}

}
