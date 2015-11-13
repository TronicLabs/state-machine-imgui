﻿#pragma once
#include <imgui.h>
#include <ccomplex>

#include "Transition.h"
#include "StateMachineLayer.h"
inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs);
inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs);
inline ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs);
inline ImVec2 operator*(const ImVec2& lhs, const double& factor);
inline ImVec2 operator*(const double& factor, const ImVec2& rhs);
inline bool operator<(const ImVec2& lhs, const ImVec2& rhs);

/* you must construct this class statically, otherwise it won't work through different frames */
class StateMachineInteraction
{
private:
	/* select/hover operation and right click operation */
	LayerID layer_selected = -1;

	StateID state_hovered_in_list = -1;
	StateID state_hovered_in_scene = -1;
	StateID state_widget_hovered = -1;

	StateID state_selected = -1;
	TransitionID trans_selected = -1;
	bool open_context_menu = false;

public:
	/* canvas attributes */


	bool node_debug = true;

	const int NUM_LAYERS = 4;

	bool hasDrawingLine = false;
	StateID transition_start_id = -1;
	ImVec2 drawing_line_start, drawing_line_end;
	ImColor darwing_line_color = ImColor(200, 50, 50);


	StateMachineInteraction() {}

	/* get the idx layer, 0 is the front, -1 is the last layer*/
	int layer(const int& idx) const {
		if (idx == -1) return 0;
		int _res = NUM_LAYERS - 1 - idx;
		return _res >= 0 ? _res : NUM_LAYERS - 1;
	}

	/* some temperory field should be cleared. */
	void updateFrame() {
		open_context_menu = false;
		state_hovered_in_list = state_hovered_in_scene = state_widget_hovered = -1;
	}

	void selectState(StateID id) {
		state_selected = id;
		trans_selected = -1;
	}

	void selectLayer(LayerID id) {
		if (layer_selected != -1 && layer_selected != id)
			state_selected = trans_selected = -1;

		layer_selected = id;
	}

	void hoverStateList(StateID id) { state_hovered_in_list = id; }

	void hoverStateScene(StateID id) { state_hovered_in_scene = id; }

	void selectTrans(TransitionID id) {
		trans_selected = id;
		state_selected = -1;
	}

	void cancelSelectTrans() { trans_selected = -1; }

	LayerID getLayerSelected() const {
		return layer_selected;
	}

	StateID getStateHoveredInList() const
	{
		return state_hovered_in_list;
	}

	StateID getStateHoveredInScene() const
	{
		return state_hovered_in_scene;
	}

	StateID getStateSelected() const
	{
		return state_selected;
	}

	TransitionID getTransSelected() const
	{
		return trans_selected;
	}

	/* if right clicked set it open */
	void setContextMenu()
	{
		if (ImGui::IsMouseClicked(1))
			this->open_context_menu = true;
	}

	bool isContextMenuOpen() const
	{
		return open_context_menu;
	}


	StateID getStateWidgetHovered() const
	{
		return state_widget_hovered;
	}

	void setStateWidgetHovered(StateID _state_widget_hovered)
	{
		this->state_widget_hovered = _state_widget_hovered;
	}

	void checkWindowRightClick()
	{
		if (!ImGui::IsAnyItemHovered() && ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1))
		{
			state_selected = state_hovered_in_list = trans_selected = -1;
			open_context_menu = true;
		}
	}

	/* the following two function shouldn't be here, move to painter later. */
	static void drawTriangleOnLine(ImDrawList& draw_list, ImVec2 from_pos, ImVec2 to_pos, ImColor color, const double arrow_width = 6)
	{
		const double arrow_height = 10;

		ImVec2 t1, t2, t3;
		ImVec2 along, perp;

		auto norm = [](ImVec2 vec) -> ImVec2
		{
			double len = std::sqrt(vec.x * vec.x + vec.y * vec.y);
			return vec * (1 / len);
		};

		along = norm(to_pos - from_pos);
		perp = ImVec2(-along.y, along.x);

		t1 = (from_pos + to_pos) * 0.5;
		t2 = t1 - along * arrow_height + perp * arrow_width;
		t3 = t1 - along * arrow_height - perp * arrow_width;

		draw_list.AddTriangleFilled(t1, t2, t3, color);
	}

	static void darwSingleTriangle(ImDrawList& draw_list, const ImVec2 anchor_pos, const ImColor color) {
		const double arrow_height = 10, arrow_width = 6;

		ImVec2 t1, t2, t3;
		t1 = anchor_pos;
		t2 = anchor_pos + ImVec2(arrow_width, arrow_height);
		t3 = anchor_pos + ImVec2(-arrow_width, arrow_height);

		draw_list.AddTriangleFilled(t1, t2, t3, color);
	}
};
