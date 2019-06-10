/**
 * This Java Class is part of the Impro-Visor Application
 *
 * Copyright (C) 2016-2019 Robert Keller and Harvey Mudd College
 *
 * Impro-Visor is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Impro-Visor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Impro-Visor; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

package imp.gui;

/**
 *
 * @author keller
 */
public class QuantizationDialog extends javax.swing.JDialog
  {
  Notate notate;
    /**
     * Creates new form QuantizationDialog
     */
    public QuantizationDialog(Notate notate, boolean modal)
    {
        super(notate, modal);
        this.notate = notate;
        initComponents();
        quantizationSpinner.setModel(new javax.swing.SpinnerListModel(notate.getQuantumString()));
        quantizationSpinner.setValue(notate.getInitialQuantumString());
    }

    /**
     * This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {
        java.awt.GridBagConstraints gridBagConstraints;

        eighthNoteSwingBox = new javax.swing.JCheckBox();
        quantizeButton = new javax.swing.JButton();
        quantizationSpinner = new javax.swing.JSpinner();

        setTitle("Requantization");
        setAlwaysOnTop(true);
        setBounds(new java.awt.Rectangle(650, 25, 0, 0));
        setMinimumSize(new java.awt.Dimension(408, 125));
        setPreferredSize(new java.awt.Dimension(408, 125));
        getContentPane().setLayout(new java.awt.GridBagLayout());

        eighthNoteSwingBox.setText("Swing Eighth-Notes");
        eighthNoteSwingBox.setToolTipText("");
        eighthNoteSwingBox.setMaximumSize(new java.awt.Dimension(158, 50));
        eighthNoteSwingBox.setMinimumSize(new java.awt.Dimension(158, 50));
        eighthNoteSwingBox.setPreferredSize(new java.awt.Dimension(158, 50));
        eighthNoteSwingBox.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                eighthNoteSwingBoxActionPerformed(evt);
            }
        });
        gridBagConstraints = new java.awt.GridBagConstraints();
        gridBagConstraints.gridx = 1;
        gridBagConstraints.gridy = 0;
        gridBagConstraints.anchor = java.awt.GridBagConstraints.WEST;
        gridBagConstraints.insets = new java.awt.Insets(10, 0, 0, 0);
        getContentPane().add(eighthNoteSwingBox, gridBagConstraints);

        quantizeButton.setBackground(new java.awt.Color(0, 255, 0));
        quantizeButton.setText("Requantize to New Chorus");
        quantizeButton.setToolTipText("Requantize the current chorus according to the set parameters, resulting in a new chorus that will be added at the end.");
        quantizeButton.setBorder(javax.swing.BorderFactory.createBevelBorder(javax.swing.border.BevelBorder.RAISED));
        quantizeButton.setContentAreaFilled(false);
        quantizeButton.setMaximumSize(new java.awt.Dimension(210, 40));
        quantizeButton.setMinimumSize(new java.awt.Dimension(210, 40));
        quantizeButton.setOpaque(true);
        quantizeButton.setPreferredSize(new java.awt.Dimension(210, 40));
        quantizeButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                quantizeButtonActionPerformed(evt);
            }
        });
        gridBagConstraints = new java.awt.GridBagConstraints();
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 1;
        gridBagConstraints.gridwidth = 2;
        gridBagConstraints.fill = java.awt.GridBagConstraints.BOTH;
        gridBagConstraints.insets = new java.awt.Insets(5, 5, 5, 5);
        getContentPane().add(quantizeButton, gridBagConstraints);

        quantizationSpinner.setModel(new javax.swing.SpinnerListModel(new String[] {"Item 0", "Item 1", "Item 2", "Item 3"}));
        quantizationSpinner.setBorder(javax.swing.BorderFactory.createTitledBorder("Quantization Level"));
        quantizationSpinner.setMinimumSize(new java.awt.Dimension(250, 50));
        quantizationSpinner.setPreferredSize(new java.awt.Dimension(250, 50));
        gridBagConstraints = new java.awt.GridBagConstraints();
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 0;
        getContentPane().add(quantizationSpinner, gridBagConstraints);

        pack();
    }// </editor-fold>//GEN-END:initComponents

    
   static int quantum[] = {20, 30, 40, 60, 120, 180, 240, 360, 480};
    
    static String quantumString[] =             
      {
          "sixteenth note triplet",
          "sixteenth note",
          "eighth note triplet",
          "eighth note",
          "quarternote ",
          "dotted quarter note",
          "half note",
          "dotted half note",
          "whole note"          
      };
    
    static String intialQuantumString = "eighth note";
    
    public boolean getSwing()
    {
        return eighthNoteSwingBox.isSelected();
    }

    public void hideQuantizeButton()
    {
        quantizeButton.setVisible(false);
    }
    

int getSelectedQuantumIndex()
    {
        return notate.getQuantumIndex(getQuantumString());
    }      

int getSelectedQuantumValue()
{
    return notate.quantum[getSelectedQuantumIndex()];
}
    
  public String getQuantumString()
  {
      return (String)quantizationSpinner.getValue();
  }
    
    private void eighthNoteSwingBoxActionPerformed(java.awt.event.ActionEvent evt)//GEN-FIRST:event_eighthNoteSwingBoxActionPerformed
    {//GEN-HEADEREND:event_eighthNoteSwingBoxActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_eighthNoteSwingBoxActionPerformed

    private void quantizeButtonActionPerformed(java.awt.event.ActionEvent evt)//GEN-FIRST:event_quantizeButtonActionPerformed
    {//GEN-HEADEREND:event_quantizeButtonActionPerformed
         notate.quantizeCurrentMelody(getSelectedQuantumIndex(), eighthNoteSwingBox.isSelected());
    }//GEN-LAST:event_quantizeButtonActionPerformed

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JCheckBox eighthNoteSwingBox;
    private javax.swing.JSpinner quantizationSpinner;
    private javax.swing.JButton quantizeButton;
    // End of variables declaration//GEN-END:variables
  }
