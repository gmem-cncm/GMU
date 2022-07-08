{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 7,
			"minor" : 3,
			"revision" : 4,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"rect" : [ 34.0, 79.0, 997.0, 709.0 ],
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
					"fontsize" : 12.710889,
					"hidden" : 1,
					"id" : "obj-32",
					"maxclass" : "newobj",
					"numinlets" : 3,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 3,
							"revision" : 4,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"rect" : [ 25.0, 69.0, 640.0, 480.0 ],
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
						"boxes" : [ 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-10",
									"index" : 3,
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 419.0, 63.0, 25.0, 25.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-9",
									"index" : 2,
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 292.0, 64.0, 25.0, 25.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-8",
									"index" : 2,
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 370.0, 209.0, 25.0, 25.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-7",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 420.0, 124.0, 91.0, 20.0 ],
									"style" : "",
									"text" : "prepend library"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-1",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 292.0, 124.0, 91.0, 20.0 ],
									"style" : "",
									"text" : "prepend preset"
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-6",
									"index" : 1,
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 178.0, 209.0, 25.0, 25.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-5",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 178.0, 127.0, 63.0, 20.0 ],
									"style" : "",
									"text" : "delay 300"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-4",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 178.0, 98.0, 24.0, 20.0 ],
									"style" : "",
									"text" : "t b"
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-3",
									"index" : 1,
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"patching_rect" : [ 178.0, 66.0, 25.0, 25.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-2",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 178.0, 161.0, 37.0, 18.0 ],
									"style" : "",
									"text" : "set 0"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-8", 0 ],
									"source" : [ "obj-1", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"source" : [ "obj-10", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"source" : [ "obj-2", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-4", 0 ],
									"source" : [ "obj-3", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"source" : [ "obj-4", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-2", 0 ],
									"source" : [ "obj-5", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-8", 0 ],
									"source" : [ "obj-7", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"source" : [ "obj-9", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 732.0, 520.472168, 37.0, 23.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p"
				}

			}
, 			{
				"box" : 				{
					"allowdrag" : 0,
					"arrow" : 0,
					"id" : "obj-33",
					"items" : [ "(library)", ",", "open", ",", "reopen", ",", "save", ",", "saveas", ",", "clear" ],
					"maxclass" : "umenu",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 732.0, 567.5, 54.0, 22.0 ],
					"prefix_mode" : 1,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"allowdrag" : 0,
					"arrow" : 0,
					"id" : "obj-36",
					"items" : [ "(preset)", ",", "new", ",", "update", ",", "rename", ",", "delete", ",", "up", ",", "down" ],
					"maxclass" : "umenu",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 732.0, 546.5, 54.0, 22.0 ],
					"prefix_mode" : 1,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.710889,
					"hidden" : 1,
					"id" : "obj-236",
					"maxclass" : "newobj",
					"numinlets" : 3,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 3,
							"revision" : 4,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"rect" : [ 25.0, 69.0, 640.0, 480.0 ],
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
						"boxes" : [ 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-10",
									"index" : 3,
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 419.0, 63.0, 25.0, 25.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-9",
									"index" : 2,
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 292.0, 64.0, 25.0, 25.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-8",
									"index" : 2,
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 370.0, 209.0, 25.0, 25.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-7",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 420.0, 124.0, 91.0, 20.0 ],
									"style" : "",
									"text" : "prepend library"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-1",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 292.0, 124.0, 91.0, 20.0 ],
									"style" : "",
									"text" : "prepend preset"
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-6",
									"index" : 1,
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 178.0, 209.0, 25.0, 25.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-5",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 178.0, 127.0, 63.0, 20.0 ],
									"style" : "",
									"text" : "delay 300"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-4",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 178.0, 98.0, 24.0, 20.0 ],
									"style" : "",
									"text" : "t b"
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-3",
									"index" : 1,
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"patching_rect" : [ 178.0, 66.0, 25.0, 25.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-2",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 178.0, 161.0, 37.0, 18.0 ],
									"style" : "",
									"text" : "set 0"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-8", 0 ],
									"source" : [ "obj-1", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"source" : [ "obj-10", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"source" : [ "obj-2", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-4", 0 ],
									"source" : [ "obj-3", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"source" : [ "obj-4", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-2", 0 ],
									"source" : [ "obj-5", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-8", 0 ],
									"source" : [ "obj-7", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"source" : [ "obj-9", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 581.0, 520.472168, 37.0, 23.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p"
				}

			}
, 			{
				"box" : 				{
					"allowdrag" : 0,
					"arrow" : 0,
					"id" : "obj-231",
					"items" : [ "(library)", ",", "open", ",", "reopen", ",", "save", ",", "saveas", ",", "clear" ],
					"maxclass" : "umenu",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 581.0, 567.5, 54.0, 22.0 ],
					"prefix_mode" : 1,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"allowdrag" : 0,
					"arrow" : 0,
					"id" : "obj-34",
					"items" : [ "(preset)", ",", "new", ",", "update", ",", "rename", ",", "delete", ",", "up", ",", "down" ],
					"maxclass" : "umenu",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 581.0, 546.5, 54.0, 22.0 ],
					"prefix_mode" : 1,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-42",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 657.0, 576.0, 37.0, 20.0 ],
					"style" : "",
					"text" : "env"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-40",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 543.0, 574.0, 37.0, 20.0 ],
					"style" : "",
					"text" : "bank"
				}

			}
, 			{
				"box" : 				{
					"allowdrag" : 0,
					"id" : "obj-38",
					"items" : [ "(gmu.env.default.json)", ",", 1, "-", "tic-tic", ",", 2, "-", "expo", "sinsoidÂ?e", ",", 3, "-", "expo", "decay", ",", 4, "-", "expo", "sinux", 2, ",", 5, "-", "sweep", "mod", ",", 6, "-", "expo", "bump", ",", 7, "-", "deux", "freq", ",", 8, "-", "sweep", "gratt", ",", 9, "-", "bpf", "sin", 1, ",", 10, "-", "bpf", "trapez", ",", 11, "-", "bpf", "sin", 2, ",", 12, "-", "blobby", ",", 13, "-", "gratz", "blob", ",", 14, "-", "crickss", ",", 15, "-", "sin", "noise", ",", 16, "-", "sin", "noise", 3, ",", 17, "-", "sin", "noise", 2, ",", 18, "-", "gratz", "blob", "rehauss", ",", 19, "-", "gauss_bruitend", ",", 20, "-", "gauss_bruitend_rept", ",", 21, "-", "bpf", "trapez", "rept", ",", 22, "-", "cluster", "bruiteux", ",", 23, "-", "clust", "bruit", "deux", ",", 24, "-", "peigne-gauss" ],
					"maxclass" : "umenu",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 647.0, 594.0, 141.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-35",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 478.0, 575.0, 38.0, 19.0 ],
					"style" : "",
					"text" : "wopen"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 1,
					"fontname" : "Arial",
					"fontsize" : 10.106784,
					"id" : "obj-37",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 431.0, 623.433289, 97.0, 18.0 ],
					"style" : "",
					"text" : "DOUBLE CLICK>>",
					"textcolor" : [ 0.290196, 0.611765, 0.380392, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-39",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 434.0, 577.0, 39.0, 19.0 ],
					"style" : "",
					"text" : "update"
				}

			}
, 			{
				"box" : 				{
					"allowdrag" : 0,
					"id" : "obj-41",
					"items" : [ "(gmu.env.bank.json)", ",", 1, "-", "bank-1" ],
					"maxclass" : "umenu",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 533.0, 593.0, 99.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-43",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 5,
					"outlettype" : [ "", "", "", "", "" ],
					"patching_rect" : [ 533.0, 620.264099, 214.0, 22.0 ],
					"style" : "",
					"text" : "gmu.envelope.editor env1 4096",
					"varname" : "Envelope_editor"
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
					"style" : "",
					"text" : "mtof~"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"hidden" : 1,
					"id" : "obj-2",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 432.0, 388.0, 40.0, 19.0 ],
					"style" : "",
					"text" : "dbtoa~"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-3",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 862.0, 517.0, 35.0, 19.0 ],
					"style" : "",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"triscale" : 0.9
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-4",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 862.0, 495.0, 70.0, 19.0 ],
					"style" : "",
					"text" : "speedlim 200"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-5",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 862.0, 474.0, 66.0, 19.0 ],
					"style" : "",
					"text" : "route voices"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-11",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 159.0, 466.0, 39.0, 19.0 ],
					"style" : "",
					"text" : "update"
				}

			}
, 			{
				"box" : 				{
					"autorestore" : "syngran_sto.xml",
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-12",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 33.0, 573.0, 177.0, 19.0 ],
					"saved_object_attributes" : 					{
						"client_rect" : [ 10, 59, 873, 585 ],
						"parameter_enable" : 0,
						"storage_rect" : [ 0, 0, 640, 240 ]
					}
,
					"style" : "",
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
					"id" : "obj-13",
					"lockeddragscroll" : 0,
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
					"id" : "obj-14",
					"maxclass" : "newobj",
					"numinlets" : 4,
					"numoutlets" : 0,
					"patching_rect" : [ 265.0, 583.0, 90.0, 19.0 ],
					"style" : "",
					"text" : "dac~ 1 2 3 4"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-15",
					"inc" : 1.071519,
					"interpinlet" : 1,
					"maxclass" : "gain~",
					"numinlets" : 2,
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 344.0, 482.0, 11.0, 81.0 ],
					"scale" : 7.94321,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-16",
					"maxclass" : "bargraf~",
					"numinlets" : 2,
					"numoutlets" : 3,
					"outlettype" : [ "float", "float", "float" ],
					"patching_rect" : [ 356.0, 482.0, 13.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-17",
					"inc" : 1.071519,
					"interpinlet" : 1,
					"maxclass" : "gain~",
					"numinlets" : 2,
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 317.0, 482.0, 11.0, 81.0 ],
					"scale" : 7.94321,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-18",
					"maxclass" : "bargraf~",
					"numinlets" : 2,
					"numoutlets" : 3,
					"outlettype" : [ "float", "float", "float" ],
					"patching_rect" : [ 329.0, 482.0, 13.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-19",
					"inc" : 1.071519,
					"interpinlet" : 1,
					"maxclass" : "gain~",
					"numinlets" : 2,
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 289.0, 482.0, 11.0, 81.0 ],
					"scale" : 7.94321,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-20",
					"maxclass" : "bargraf~",
					"numinlets" : 2,
					"numoutlets" : 3,
					"outlettype" : [ "float", "float", "float" ],
					"patching_rect" : [ 301.0, 482.0, 13.0, 93.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-21",
					"inc" : 1.071519,
					"interpinlet" : 1,
					"maxclass" : "gain~",
					"numinlets" : 2,
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 261.0, 482.0, 11.0, 81.0 ],
					"scale" : 7.94321,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-22",
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
					"id" : "obj-23",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 42.0, 424.0, 35.0, 19.0 ],
					"style" : "",
					"text" : "poll 1"
				}

			}
, 			{
				"box" : 				{
					"args" : [ "pan", 0.07, 0.0, 1, 20.0, 1, "0./1." ],
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-24",
					"lockeddragscroll" : 0,
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
					"id" : "obj-25",
					"lockeddragscroll" : 0,
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
					"id" : "obj-26",
					"lockeddragscroll" : 0,
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
					"id" : "obj-27",
					"lockeddragscroll" : 0,
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
					"id" : "obj-28",
					"lockeddragscroll" : 0,
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
					"id" : "obj-29",
					"maxclass" : "ezdac~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 212.0, 532.0, 33.0, 33.0 ],
					"style" : ""
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
					"id" : "obj-30",
					"lockeddragscroll" : 0,
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
					"id" : "obj-31",
					"maxclass" : "newobj",
					"numinlets" : 7,
					"numoutlets" : 5,
					"outlettype" : [ "signal", "signal", "signal", "signal", "" ],
					"patching_rect" : [ 20.0, 446.0, 737.0, 19.0 ],
					"style" : "",
					"text" : "synGranul~ env123 4"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 1 ],
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-13", 1 ],
					"midpoints" : [ 168.5, 488.0, 119.0, 488.0 ],
					"source" : [ "obj-11", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-13", 0 ],
					"hidden" : 1,
					"source" : [ "obj-12", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-12", 0 ],
					"hidden" : 1,
					"source" : [ "obj-13", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-14", 3 ],
					"order" : 1,
					"source" : [ "obj-15", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"hidden" : 1,
					"order" : 0,
					"source" : [ "obj-15", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-14", 2 ],
					"order" : 1,
					"source" : [ "obj-17", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-18", 0 ],
					"hidden" : 1,
					"order" : 0,
					"source" : [ "obj-17", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-14", 1 ],
					"order" : 1,
					"source" : [ "obj-19", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-20", 0 ],
					"hidden" : 1,
					"order" : 0,
					"source" : [ "obj-19", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 3 ],
					"source" : [ "obj-2", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-14", 0 ],
					"order" : 1,
					"source" : [ "obj-21", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-15", 0 ],
					"hidden" : 1,
					"order" : 0,
					"source" : [ "obj-21", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"hidden" : 1,
					"order" : 1,
					"source" : [ "obj-21", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-19", 0 ],
					"hidden" : 1,
					"order" : 2,
					"source" : [ "obj-21", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-22", 0 ],
					"hidden" : 1,
					"order" : 0,
					"source" : [ "obj-21", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"source" : [ "obj-23", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-236", 2 ],
					"hidden" : 1,
					"midpoints" : [ 608.0, 589.0, 637.0, 589.0, 637.0, 517.0, 608.5, 517.0 ],
					"source" : [ "obj-231", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-236", 0 ],
					"hidden" : 1,
					"source" : [ "obj-231", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-231", 0 ],
					"hidden" : 1,
					"order" : 0,
					"source" : [ "obj-236", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-34", 0 ],
					"hidden" : 1,
					"order" : 1,
					"source" : [ "obj-236", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-43", 0 ],
					"hidden" : 1,
					"midpoints" : [ 608.5, 617.0, 542.5, 617.0 ],
					"source" : [ "obj-236", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 5 ],
					"source" : [ "obj-24", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 4 ],
					"source" : [ "obj-25", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-2", 0 ],
					"hidden" : 1,
					"source" : [ "obj-26", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 2 ],
					"source" : [ "obj-27", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"source" : [ "obj-28", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"source" : [ "obj-30", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-15", 0 ],
					"source" : [ "obj-31", 3 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-31", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-19", 0 ],
					"midpoints" : [ 209.0, 475.0, 292.5, 475.0 ],
					"source" : [ "obj-31", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-21", 0 ],
					"midpoints" : [ 29.5, 475.0, 264.5, 475.0 ],
					"source" : [ "obj-31", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-33", 0 ],
					"hidden" : 1,
					"order" : 0,
					"source" : [ "obj-32", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-36", 0 ],
					"hidden" : 1,
					"order" : 1,
					"source" : [ "obj-32", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-43", 1 ],
					"hidden" : 1,
					"midpoints" : [ 759.5, 620.0, 737.5, 620.0 ],
					"source" : [ "obj-32", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 2 ],
					"hidden" : 1,
					"midpoints" : [ 759.0, 589.0, 788.0, 589.0, 788.0, 517.0, 759.5, 517.0 ],
					"source" : [ "obj-33", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 0 ],
					"hidden" : 1,
					"source" : [ "obj-33", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-236", 1 ],
					"hidden" : 1,
					"midpoints" : [ 608.0, 589.0, 637.0, 589.0, 637.0, 517.0, 599.5, 517.0 ],
					"source" : [ "obj-34", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-236", 0 ],
					"hidden" : 1,
					"source" : [ "obj-34", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-43", 0 ],
					"midpoints" : [ 487.5, 618.0, 542.5, 618.0 ],
					"source" : [ "obj-35", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 1 ],
					"hidden" : 1,
					"midpoints" : [ 759.0, 589.0, 788.0, 589.0, 788.0, 517.0, 750.5, 517.0 ],
					"source" : [ "obj-36", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 0 ],
					"hidden" : 1,
					"source" : [ "obj-36", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-43", 1 ],
					"midpoints" : [ 656.5, 616.13205, 737.5, 616.13205 ],
					"source" : [ "obj-38", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-43", 0 ],
					"midpoints" : [ 443.5, 618.0, 542.5, 618.0 ],
					"source" : [ "obj-39", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-3", 0 ],
					"source" : [ "obj-4", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-43", 0 ],
					"source" : [ "obj-41", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"source" : [ "obj-43", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-38", 0 ],
					"midpoints" : [ 688.75, 647.0, 826.0, 647.0, 826.0, 539.0, 656.5, 539.0 ],
					"source" : [ "obj-43", 3 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-41", 0 ],
					"midpoints" : [ 640.0, 655.0, 833.0, 655.0, 833.0, 535.0, 542.5, 535.0 ],
					"source" : [ "obj-43", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-4", 0 ],
					"source" : [ "obj-5", 0 ]
				}

			}
 ],
		"parameters" : 		{
			"obj-43::obj-229" : [ "live.tab", "live.tab", 0 ]
		}
,
		"dependency_cache" : [ 			{
				"name" : "trig_ctrl5.maxpat",
				"bootpath" : "~/Documents/_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs",
				"patcherrelativepath" : "../../../../../_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "mslide_XY2MS.maxpat",
				"bootpath" : "~/Documents/_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs",
				"patcherrelativepath" : "../../../../../_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "sw.maxpat",
				"bootpath" : "~/Documents/_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/_abs",
				"patcherrelativepath" : "../../../../../_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/_abs",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "mysw.maxpat",
				"bootpath" : "~/Documents/_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs",
				"patcherrelativepath" : "../../../../../_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "mychange.maxpat",
				"bootpath" : "~/Documents/_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs",
				"patcherrelativepath" : "../../../../../_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "setname.js",
				"bootpath" : "~/Documents/_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/js",
				"patcherrelativepath" : "../../../../../_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/js",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "tranche_ctrl5.maxpat",
				"bootpath" : "~/Documents/_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs",
				"patcherrelativepath" : "../../../../../_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "ali.pattr-ui.js",
				"bootpath" : "~/Documents/_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/js/_3rd",
				"patcherrelativepath" : "../../../../../_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/js/_3rd",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "storage_helper.maxpat",
				"bootpath" : "~/Documents/_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs",
				"patcherrelativepath" : "../../../../../_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "dialog_preset.maxpat",
				"bootpath" : "~/Documents/_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs/Envelope_Edit",
				"patcherrelativepath" : "../../../../../_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs/Envelope_Edit",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "syngran_sto.xml",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/GMU-1.6.1-max7/examples",
				"patcherrelativepath" : ".",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "gmu.envelope.editor.maxpat",
				"bootpath" : "~/Documents/_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs/Envelope_Edit",
				"patcherrelativepath" : "../../../../../_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs/Envelope_Edit",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "ad.toggle2.png",
				"bootpath" : "~/Documents/_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs/Envelope_Edit",
				"patcherrelativepath" : "../../../../../_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs/Envelope_Edit",
				"type" : "PNG ",
				"implicit" : 1
			}
, 			{
				"name" : "ad.toggle.png",
				"bootpath" : "~/Documents/_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs/Envelope_Edit",
				"patcherrelativepath" : "../../../../../_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs/Envelope_Edit",
				"type" : "PNG ",
				"implicit" : 1
			}
, 			{
				"name" : "gmu.env.bank.json",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/GMU-1.6.1-max7/abs/Envelope_Edit",
				"patcherrelativepath" : "../abs/Envelope_Edit",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "gmu.poly.buffer.maxpat",
				"bootpath" : "~/Documents/_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs/Envelope_Edit",
				"patcherrelativepath" : "../../../../../_GMEM/_assist/Verunelli/UNFolding/_UNfolding-RT-Patch-work-64bit/_add64/GMU-1.6.1-max7/abs/Envelope_Edit",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "gmu.env.default.json",
				"bootpath" : "~/Documents/_Max_lib/gmem-ajout/+subfolder/GMU-1.6.1-max7/abs/Envelope_Edit",
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
