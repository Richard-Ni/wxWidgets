/////////////////////////////////////////////////////////////////////////////
// Name:        constraints.h
// Purpose:     topic overview
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/*!

 @page overview_constraints Constraints overview

 Classes: wxLayoutConstraints, wxIndividualLayoutConstraint.

 @note Constraints are now deprecated and you should use sizers instead (see wxSizer).

 Objects of class wxLayoutConstraint can be associated with a window to define
 the way it is laid out, with respect to its siblings or the parent.

 The class consists of the following eight constraints of class wxIndividualLayoutConstraint,
 some or all of which should be accessed directly to set the appropriate
 constraints.

  @b left: represents the left hand edge of the window
  @b right: represents the right hand edge of the window
  @b top: represents the top edge of the window
  @b bottom: represents the bottom edge of the window
  @b width: represents the width of the window
  @b height: represents the height of the window
  @b centreX: represents the horizontal centre point of the window
  @b centreY: represents the vertical centre point of the window

 The constraints are initially set to have the relationship wxUnconstrained,
 which means that their values should be calculated by looking at known constraints.
 To calculate the position and size of the control, the layout algorithm needs to
 know exactly 4 constraints (as it has 4 numbers to calculate from them), so you
 should always set exactly 4 of the constraints from the above table.

 If you want the controls height or width to have the default value, you may use
 a special value for the constraint: wxAsIs. If the constraint is wxAsIs, the
 dimension will not be changed which is useful for the dialog controls which
 often have the default size (e.g. the buttons whose size is determined by their
 label).

 The constrains calculation is done in wxWindow::Layout function which evaluates 
 constraints. To call it you can either call wxWindow::SetAutoLayout if the parent 
 window is a frame, panel or a dialog to tell default OnSize handlers to call Layout
 automatically whenever the window size changes, or override OnSize and call
 Layout yourself (note that you do have to call wxWindow::Layout yourself if the parent 
 window is not a frame, panel or dialog).

 @li @ref overview_constraints_layout
 @li @ref overview_constraints_examples


 <hr>


 @section overview_constraints_layout Constraint layout: more details

 By default, windows do not have a wxLayoutConstraints object. In this case, much layout
 must be done explicitly, by performing calculations in OnSize members, except
 for the case of frames that have exactly one subwindow (not counting toolbar and
 statusbar which are also positioned by the frame automatically), where wxFrame::OnSize
 takes care of resizing the child to always fill the frame.

 To avoid the need for these rather awkward calculations, the user can create
 a wxLayoutConstraints object and associate it with a window with wxWindow::SetConstraints.
 This object contains a constraint for each of the window edges, two for the centre point,
 and two for the window size. By setting some or all of these constraints appropriately,
 the user can achieve quite complex layout by defining relationships between windows.

 In wxWidgets, each window can be constrained relative to either its @e siblings 
 on the same window, or the @e parent. The layout algorithm
 therefore operates in a top-down manner, finding the correct layout for
 the children of a window, then the layout for the grandchildren, and so on. 

 Note that this differs markedly from native Motif layout, where
 constraints can ripple upwards and can eventually change the frame
 window or dialog box size. We assume in wxWidgets that the @e user is
 always 'boss' and specifies the size of the outer window, to which
 subwindows must conform. Obviously, this might be a limitation in some
 circumstances, but it suffices for most situations, and the
 simplification avoids some of the nightmarish problems associated with
 programming Motif.

 When the user sets constraints, many of the constraints for windows
 edges and dimensions remain unconstrained. For a given window,
 the wxWindow::Layout algorithm first resets all constraints
 in all children to have unknown edge or dimension values, and then iterates 
 through the constraints, evaluating them. For unconstrained edges and dimensions,
 it tries to find the value using known relationships that always hold. For example,
 an unconstrained @e width may be calculated from the @e left and @e right edges, if
 both are currently known. For edges and dimensions with user-supplied constraints, these
 constraints are evaluated if the inputs of the constraint are known.

 The algorithm stops when all child edges and dimension are known (success), or
 there are unknown edges or dimensions but there has been no change in this cycle (failure).
 It then sets all the window positions and sizes according to the values it has found.
 Because the algorithm is iterative, the order in which constraints are considered is
 irrelevant, however you may reduce the number of iterations (and thus speed up
 the layout calculations) by creating the controls in such order that as many
 constraints as possible can be calculated during the first iteration. For example, if
 you have 2 buttons which you'd like to position in the lower right corner, it is
 slightly more efficient to first create the second button and specify that its
 right border IsSameAs(parent, wxRight) and then create the first one by
 specifying that it should be LeftOf() the second one than to do in a more
 natural left-to-right order.



 @section overview_constraints_examples Window layout examples

 @subsection overview_constraints_example1 Example 1: subwindow layout

 This example specifies a panel and a window side by side,
 with a text subwindow below it.

 @code
   frame->panel = new wxPanel(frame, -1, wxPoint(0, 0), wxSize(1000, 500), 0);
   frame->scrollWindow = new MyScrolledWindow(frame, -1, wxPoint(0, 0), wxSize(400, 400), wxRETAINED);
   frame->text_window = new MyTextWindow(frame, -1, wxPoint(0, 250), wxSize(400, 250));

   // Set constraints for panel subwindow
   wxLayoutConstraints *c1 = new wxLayoutConstraints;

   c1->left.SameAs       (frame, wxLeft);
   c1->top.SameAs        (frame, wxTop);
   c1->right.PercentOf   (frame, wxWidth, 50);
   c1->height.PercentOf  (frame, wxHeight, 50);

   frame->panel->SetConstraints(c1);

   // Set constraints for scrollWindow subwindow
   wxLayoutConstraints *c2 = new wxLayoutConstraints;

   c2->left.SameAs       (frame->panel, wxRight);
   c2->top.SameAs        (frame, wxTop);
   c2->right.SameAs      (frame, wxRight);
   c2->height.PercentOf  (frame, wxHeight, 50);

   frame->scrollWindow->SetConstraints(c2);

   // Set constraints for text subwindow
   wxLayoutConstraints *c3 = new wxLayoutConstraints;
   c3->left.SameAs       (frame, wxLeft);
   c3->top.Below         (frame->panel);
   c3->right.SameAs      (frame, wxRight);
   c3->bottom.SameAs     (frame, wxBottom);

   frame->text_window->SetConstraints(c3);
 @endcode


 @subsection overview_constraints_example2 Example 2: panel item layout

 This example sizes a button width to 80 percent of the panel width, and centres
 it horizontally. A listbox and multitext item are placed below it. The listbox
 takes up 40 percent of the panel width, and the multitext item takes up
 the remainder of the width. Margins of 5 pixels are used.

 @code
   // Create some panel items
   wxButton *btn1 = new wxButton(frame->panel, ->1, "A button") ;

   wxLayoutConstraints *b1 = new wxLayoutConstraints;
   b1->centreX.SameAs    (frame->panel, wxCentreX);
   b1->top.SameAs        (frame->panel, wxTop, 5);
   b1->width.PercentOf   (frame->panel, wxWidth, 80);
   b1->height.PercentOf  (frame->panel, wxHeight, 10);
   btn1->SetConstraints(b1);

   wxListBox *list = new wxListBox(frame->panel, ->1, "A list",
                                   wxPoint(->1, ->1), wxSize(200, 100));

   wxLayoutConstraints *b2 = new wxLayoutConstraints;
   b2->top.Below         (btn1, 5);
   b2->left.SameAs       (frame->panel, wxLeft, 5);
   b2->width.PercentOf   (frame->panel, wxWidth, 40);
   b2->bottom.SameAs     (frame->panel, wxBottom, 5);
   list->SetConstraints(b2);

   wxTextCtrl *mtext = new wxTextCtrl(frame->panel, ->1, "Multiline text", "Some text",
                         wxPoint(->1, ->1), wxSize(150, 100), wxTE_MULTILINE);

   wxLayoutConstraints *b3 = new wxLayoutConstraints;
   b3->top.Below         (btn1, 5);
   b3->left.RightOf      (list, 5);
   b3->right.SameAs      (frame->panel, wxRight, 5);
   b3->bottom.SameAs     (frame->panel, wxBottom, 5);
   mtext->SetConstraints(b3);
 @endcode

*/

